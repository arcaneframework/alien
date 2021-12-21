/*
 * Copyright 2021 IFPEN-CEA
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <map>
#include <mpi.h>

#include <boost/lexical_cast.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/cmdline.hpp>
#include <boost/program_options/variables_map.hpp>

#include <arccore/message_passing_mpi/StandaloneMpiMessagePassingMng.h>
#include <arccore/trace/ITraceMng.h>

#include <alien/distribution/MatrixDistribution.h>
#include <alien/distribution/VectorDistribution.h>
#include <alien/index_manager/IIndexManager.h>
#include <alien/index_manager/IndexManager.h>
#include <alien/index_manager/functional/DefaultAbstractFamily.h>

#include <alien/ref/AlienRefSemantic.h>

#include <alien/kernels/simple_csr/algebra/SimpleCSRLinearAlgebra.h>
#include <alien/kernels/simple_csr/algebra/SimpleCSRInternalLinearAlgebra.h>

#ifdef ALIEN_USE_SYCL
#include <alien/kernels/sycl/SYCLPrecomp.h>
#include <alien/kernels/sycl/data/SYCLBEllPackMatrix.h>
#include <alien/kernels/sycl/data/SYCLVector.h>
#include <alien/kernels/sycl/algebra/SYCLLinearAlgebra.h>
#include <alien/kernels/sycl/algebra/SYCLInternalLinearAlgebra.h>
#endif

#include <alien/expression/krylov/BiCGStab.h>
#include <alien/expression/krylov/DiagPreconditioner.h>
#include <alien/expression/krylov/ChebyshevPreconditioner.h>
#include <alien/expression/krylov/NeumannPolyPreconditioner.h>

#include <alien/utils/StdTimer.h>

namespace Environment
{
void initialize(int argc, char** argv)
{
  MPI_Init(&argc, &argv);
}

void finalize()
{
  MPI_Finalize();
}

Arccore::MessagePassing::IMessagePassingMng*
parallelMng()
{
  return Arccore::MessagePassing::Mpi::StandaloneMpiMessagePassingMng::create(
  MPI_COMM_WORLD);
}

Arccore::ITraceMng*
traceMng()
{
  return Arccore::arccoreCreateDefaultTraceMng();
}
} // namespace Environment

// Define index type for local ids
typedef Arccore::Integer LID;
// Define index type for global (unique) ids
typedef Arccore::Int64 UID;

int main(int argc, char** argv)
{

  using namespace boost::program_options ;
  options_description desc;
  desc.add_options()
      ("help", "produce help")
      ("nx",                  value<int>()->default_value(10),       "nx")
      ("ny",                  value<int>()->default_value(10),       "ny")
      ("precond",             value<int>()->default_value(1),        "preconditioner 0->id, 1->cheb, 2->neumann")
      ("output-level",        value<int>()->default_value(0),        "output level")
      ("max-iter",            value<int>()->default_value(1000),     "max iterations")
      ("tol",                 value<double>()->default_value(1.e-6), "Tolerance")
      ("poly-factor",         value<double>()->default_value(0.5),   "Polynome factor")
      ("poly-factor-max-iter",value<int>()->default_value(10),       "Polynome factor max iterations")
      ("poly-order",          value<int>()->default_value(3),        "Polynome order")
      ("kernel",              value<std::string>()->default_value("simplecsr"), "Kernel type simplecsr sycl") ;

  variables_map vm;
  store(parse_command_line(argc, argv, desc), vm);
  notify(vm);

  if (vm.count("help")) {
      std::cout << desc << "\n";
      return 1;
  }

  /*
     * Example : LAPLACIAN PROBLEM on a 2D square mesh of size NX x NY
     * Unknowns on nodes (i,j)
     * Use a 5-Points stencil
     *
     *
     *           (I,J+1)
     *              |
     * (I-1,J) -- (I,J) -- (I+1,J)
     *              |
     *           (I,J-1)
     *
     * TUTORIAL : LINEAR SYSTEM mat.X=rhs DEFINITION
     * =========================================
     */
  int Nx = vm["nx"].as<int>();
  int Ny = vm["ny"].as<int>();
  // int space_size = Nx * Ny;

  // INITIALIZE PARALLEL ENVIRONMENT
  Environment::initialize(argc, argv);

  auto parallel_mng = Environment::parallelMng();
  auto trace_mng = Environment::traceMng();

  auto comm_size = Environment::parallelMng()->commSize();
  auto comm_rank = Environment::parallelMng()->commRank();

  trace_mng->info() << "NB PROC = " << comm_size;
  trace_mng->info() << "RANK    = " << comm_rank;

  /*
     * MESH PARTITION ALONG Y AXIS
     *
     */
  int local_ny = Ny / comm_size;
  int r = Ny % comm_size;

  std::vector<int> y_offset(comm_size + 1);
  y_offset[0] = 0;
  for (int ip = 0; ip < r; ++ip)
    y_offset[ip + 1] = y_offset[ip] + local_ny + 1;

  for (int ip = r; ip < comm_size; ++ip)
    y_offset[ip + 1] = y_offset[ip] + local_ny;

  // Define a lambda function to compute node uids from the 2D (i,j) coordinates
  // (i,j) -> uid = node_uid(i,j)
  auto node_uid = [&](int i, int j) { return j * Ny + i; };

  /*
     * DEFINITION of Unknowns Unique Ids and  Local Ids
     */
  Alien::UniqueArray<UID> uid;
  Alien::UniqueArray<Arccore::Integer> owners;
  Alien::UniqueArray<LID> lid;
  std::map<UID, LID> uid2lid;
  int first_j = y_offset[comm_rank];
  int last_j = y_offset[comm_rank + 1];

  int index = 0;
  for (int j = first_j; j < last_j; ++j) {
    for (int i = 0; i < Nx; ++i) {
      int n_uid = node_uid(i, j);
      uid.add(n_uid);
      owners.add(comm_rank);
      lid.add(index);
      uid2lid[n_uid] = index;
      ++index;
    }
  }

  int nb_ghost = 0;
  if (comm_size > 1) {
    if (comm_rank > 0) {
      for (int i = 0; i < Nx; ++i) {
        int n_uid = node_uid(i, first_j - 1);
        uid.add(n_uid);
        owners.add(comm_rank - 1);
        lid.add(index);
        uid2lid[n_uid] = index;
        ++index;
        ++nb_ghost;
      }
    }
    if (comm_rank < comm_size - 1) {
      for (int i = 0; i < Nx; ++i) {
        int n_uid = node_uid(i, last_j + 1);
        uid.add(n_uid);
        owners.add(comm_rank + 1);
        lid.add(index);
        uid2lid[n_uid] = index;
        ++index;
        ++nb_ghost;
      }
    }
  }

  /*
     * DEFINITION of an abstract family of unknowns
     */
  Alien::DefaultAbstractFamily family(uid, owners, parallel_mng);

  Alien::IndexManager index_manager(parallel_mng);

  /*
     * Creation of a set of indexes
     */
  auto indexSetU = index_manager.buildScalarIndexSet("U", family, 0);

  // Combine all index set and create Linear system index system
  index_manager.prepare();

  auto global_size = index_manager.globalSize();
  auto local_size = index_manager.localSize();

  trace_mng->info() << "GLOBAL SIZE : " << global_size;
  trace_mng->info() << "LOCAL SIZE  : " << local_size;

  /*
     * DEFINITION of
     * - Alien Space,
     * - matrix and vector distributions
     * to manage the distribution of indexes between all MPI processes
     */

  auto space = Alien::Space(global_size, "MySpace");

  auto matrix_dist =
  Alien::MatrixDistribution(global_size, global_size, local_size, parallel_mng);
  auto vector_dist = Alien::VectorDistribution(global_size, local_size, parallel_mng);

  trace_mng->info() << "MATRIX DISTRIBUTION INFO";
  trace_mng->info() << "GLOBAL ROW SIZE : " << matrix_dist.globalRowSize();
  trace_mng->info() << "LOCAL ROW SIZE  : " << matrix_dist.localRowSize();
  trace_mng->info() << "GLOBAL COL SIZE : " << matrix_dist.globalColSize();
  trace_mng->info() << "LOCAL COL SIZE  : " << matrix_dist.localColSize();

  trace_mng->info() << "VECTOR DISTRIBUTION INFO";
  trace_mng->info() << "GLOBAL SIZE : " << vector_dist.globalSize();
  trace_mng->info() << "LOCAL SIZE  : " << vector_dist.localSize();

  auto allUIndex = index_manager.getIndexes(indexSetU);

  /*
   *  Assemble matrix.
   */
  auto A = Alien::Matrix(matrix_dist);

  /* Two passes */

  // PROFILE DEFINITION
  {
    Alien::MatrixProfiler profiler(A);

    for (int j = first_j; j < last_j; ++j) {
      // BOUCLE SUIVANT AXE X
      for (int i = 0; i < Nx; ++i) {
        auto n_uid = node_uid(i, j);
        auto n_lid = uid2lid[n_uid];
        auto irow = allUIndex[n_lid];

        // DEFINE DIAGONAL
        profiler.addMatrixEntry(irow, irow);

        // OFF DIAG
        // lower
        if (j > 0) {
          auto off_uid = node_uid(i, j - 1);
          auto off_lid = uid2lid[off_uid];
          auto jcol = allUIndex[off_lid];
          if (jcol != -1)
            profiler.addMatrixEntry(irow, jcol);
        }
        // left
        if (i > 0) {
          auto off_uid = node_uid(i - 1, j);
          auto off_lid = uid2lid[off_uid];
          auto jcol = allUIndex[off_lid];
          if (jcol != -1)
            profiler.addMatrixEntry(irow, jcol);
        }
        // right
        if (i < Nx - 1) {
          auto off_uid = node_uid(i + 1, j);
          auto off_lid = uid2lid[off_uid];
          auto jcol = allUIndex[off_lid];
          if (jcol != -1)
            profiler.addMatrixEntry(irow, jcol);
        }
        // upper
        if (j < Ny - 1) {
          auto off_uid = node_uid(i, j + 1);
          auto off_lid = uid2lid[off_uid];
          auto jcol = allUIndex[off_lid];
          if (jcol != -1)
            profiler.addMatrixEntry(irow, jcol);
        }
      }
    }
  }

  // SECOND STEP : MATRIX FILLING STEP
  {
    Alien::ProfiledMatrixBuilder builder(A, Alien::ProfiledMatrixOptions::eResetValues);
    // Loop on Y-axis
    for (int j = first_j; j < last_j; ++j) {
      // Loop on X-axis
      for (int i = 0; i < Nx; ++i) {
        auto n_uid = node_uid(i, j);
        auto n_lid = uid2lid[n_uid];
        auto irow = allUIndex[n_lid];

        // DIAGONAL
        builder(irow, irow) = 4;

        // OFF DIAG
        // lower
        if (j > 0) {
          auto off_uid = node_uid(i, j - 1);
          auto off_lid = uid2lid[off_uid];
          auto jcol = allUIndex[off_lid];
          if (jcol != -1)
            builder(irow, jcol) = -1;
        }
        // left
        if (i > 0) {
          auto off_uid = node_uid(i - 1, j);
          auto off_lid = uid2lid[off_uid];
          auto jcol = allUIndex[off_lid];
          if (jcol != -1)
            builder(irow, jcol) = -1;
        }
        // right
        if (i < Nx - 1) {
          auto off_uid = node_uid(i + 1, j);
          auto off_lid = uid2lid[off_uid];
          auto jcol = allUIndex[off_lid];
          if (jcol != -1)
            builder(irow, jcol) = -1;
        }
        // upper
        if (j < Ny - 1) {
          auto off_uid = node_uid(i, j + 1);
          auto off_lid = uid2lid[off_uid];
          auto jcol = allUIndex[off_lid];
          if (jcol != -1)
            builder(irow, jcol) = -1;
        }
      }
    }
  }


  /*
   * Build rhs vector
   */
  auto b = Alien::Vector(vector_dist);

  {
    Alien::VectorWriter writer(b);

    // Loop on Y-axis
    for (int j = first_j; j < last_j; ++j) {
      // Loop on X-axis
      for (int i = 0; i < Nx; ++i) {
        auto n_uid = node_uid(i, j);
        auto n_lid = uid2lid[n_uid];
        auto irow = allUIndex[n_lid];

        writer[irow] = 1. / (1. + i + j);
      }
    }
  }

  auto x = Alien::Vector(vector_dist);

  typedef Alien::StdTimer   TimerType ;
  typedef TimerType::Sentry SentryType ;

  TimerType timer;
  int         max_iteration = vm["max-iter"].as<int>();
  double      tol           = vm["tol"].as<double>();
  int         precond       = vm["precond"].as<int>();
  std::string kernel        = vm["kernel"].as<std::string>() ;
  int         output_level  = vm["output-level"].as<int>();

  auto run = [&](auto& alg)
            {
              typedef typename
                  boost::remove_reference<decltype(alg)>::type AlgebraType ;
              typedef typename AlgebraType::BackEndType        BackEndType ;
              typedef Alien::BiCGStab<AlgebraType>             SolverType ;
              typedef typename SolverType::Iteration           StopCriteriaType ;


              auto const& true_A = A.impl()->get<BackEndType>() ;
              auto const& true_b = b.impl()->get<BackEndType>() ;
              auto&       true_x = x.impl()->get<BackEndType>(true) ;

              SolverType       solver{alg,trace_mng} ;
              solver.setOutputLevel(output_level) ;
              StopCriteriaType stop_criteria{alg,true_b,tol,max_iteration,output_level>0?trace_mng:nullptr} ;

              switch(precond)
              {
                case 0 :
                {
                  trace_mng->info()<<"DIAG PRECONDITIONER";
                  typedef Alien::DiagPreconditioner<AlgebraType> PrecondType ;
                  PrecondType      precond{true_A} ;
                  precond.init() ;
                  SentryType sentry(timer,"BiCGS-Diag") ;
                  solver.solve(precond,stop_criteria,true_A,true_b,true_x) ;
                }
                break ;
                case 1:
                {
                  trace_mng->info()<<"CHEBYSHEV PRECONDITIONER";
                  double polynom_factor          = vm["poly-factor"].as<double>() ;
                  int    polynom_order           = vm["poly-order"].as<int>() ;
                  int    polynom_factor_max_iter = vm["poly-factor-max-iter"].as<int>() ;

                  typedef Alien::ChebyshevPreconditioner<AlgebraType> PrecondType ;
                  PrecondType      precond{alg,true_A,polynom_factor,polynom_order,polynom_factor_max_iter,trace_mng} ;
                  precond.setOutputLevel(output_level) ;
                  precond.init() ;

                  SentryType sentry(timer,"BiCGS-ChebyshevPoly") ;
                  solver.solve(precond,stop_criteria,true_A,true_b,true_x) ;
                }
                break ;
                case 2:
                {
                  trace_mng->info()<<"NEUNMAN PRECONDITIONER";
                  double polynom_factor          = vm["poly-factor"].as<double>() ;
                  int    polynom_order           = vm["poly-order"].as<int>() ;
                  int    polynom_factor_max_iter = vm["poly-factor-max-iter"].as<int>() ;

                  typedef Alien::NeumannPolyPreconditioner<AlgebraType> PrecondType ;
                  PrecondType precond{alg,true_A,polynom_factor,polynom_order,polynom_factor_max_iter,trace_mng} ;
                  precond.init() ;

                  SentryType sentry(timer,"BiCGS-NeumanPoly") ;
                  solver.solve(precond,stop_criteria,true_A,true_b,true_x) ;
                }
                break ;
                default:
                  trace_mng->info()<<"Unknown Preconitioner id";
                  break ;
              }
              if(stop_criteria.getStatus())
              {
                trace_mng->info()<<"Solver has converged";
                trace_mng->info()<<"Nb iterations  : "<<stop_criteria();
                trace_mng->info()<<"Criteria value : "<<stop_criteria.getValue();
              }
              else
              {
                trace_mng->info()<<"Solver convergence failed";
              }
            } ;

  if(kernel.compare("simplecsr")==0)
  {
    Alien::SimpleCSRInternalLinearAlgebra alg ;
    run(alg) ;
  }
  if(kernel.compare("sycl")==0)
  {
#ifdef ALIEN_USE_SYCL
    Alien::SYCLInternalLinearAlgebra alg;
    run(alg) ;
#else
    trace_mng->info()<<"SYCL BackEnd not available";
#endif
  }

  timer.printInfo() ;


  Environment::finalize();

  return 0;
}
