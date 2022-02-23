/*
 * Copyright 2020 IFPEN-CEA
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

#include <arccore/message_passing_mpi/StandaloneMpiMessagePassingMng.h>

#include <alien/move/AlienMoveSemantic.h>
#include <alien/move/handlers/scalar/VectorWriter.h>

#include <alien/benchmark/ILinearProblem.h>
#include <alien/benchmark/LinearBench.h>

#include <alien/ginkgo/backend.h>
#include <alien/ginkgo/options.h>

double vecMax(const Alien::Move::VectorData& v)
{
  Alien::Move::VectorReader R(std::move(v));
  double max = R[0];
  for (auto i = 1; i < R.size(); i++) {
    if (R[i] > max)
      max = R[i];
  }
  return max;
}

double vecMin(const Alien::Move::VectorData& v)
{
  Alien::Move::VectorReader R(std::move(v));
  double min = R[0];
  for (auto i = 1; i < R.size(); i++) {
    if (R[i] < min)
      min = R[i];
  }
  return min;
}

double vecMaxAbs(const Alien::Move::VectorData& v)
{
  Alien::Move::VectorReader R(std::move(v));
  double absmax;
  absmax = std::abs(R[0]);
  for (auto i = 1; i < R.size(); i++) {
    if (std::abs(R[i]) > absmax)
      absmax = std::abs(R[i]);
  }
  return absmax;
}

double vecMinAbs(const Alien::Move::VectorData& v)
{
  Alien::Move::VectorReader R(std::move(v));
  double absmin;
  absmin = std::abs(R[0]);
  for (auto i = 1; i < R.size(); i++) {
    if (std::abs(R[i]) < absmin)
      absmin = std::abs(R[i]);
  }
  return absmin;
}

int test(const Alien::Ginkgo::OptionTypes::eSolver& solv, const Alien::Ginkgo::OptionTypes::ePreconditioner& prec, const std::string& mat_filename, const std::string& vec_filename)
{
  auto* pm = Arccore::MessagePassing::Mpi::StandaloneMpiMessagePassingMng::create(MPI_COMM_WORLD);
  auto* tm = Arccore::arccoreCreateDefaultTraceMng();
  Alien::setTraceMng(tm);

  auto problem = Alien::Benchmark::buildFromMatrixMarket(pm, mat_filename, vec_filename);

  Alien::Ginkgo::Options options;
  options.numIterationsMax(500);
  options.stopCriteriaValue(1e-8);
  options.preconditioner(prec); // Jacobi, NoPC
  options.solver(solv); //CG, GMRES, BICG, BICGSTAB
  auto solver = Alien::Ginkgo::LinearSolver(options);

  auto bench = Alien::Benchmark::LinearBench(std::move(problem));

  bench.solve(&solver);
//
//
//
//  xe.distribution();
//
//  /**
//	 *  Vecteur b
//	 *************/
//
//  auto b = problem->vector();
//
//  /**
//	 *  Préparation du solveur pour le calcul de x, tq : Ax = b
//	 ********************************************/
//  Alien::Move::VectorData x(A.colSpace(), A.distribution().rowDistribution());

//
//  /**
//	 *  BENCH
//	 ********************************************/
//
//  int nbRuns = 5;
//  for (int i = 0; i < nbRuns; i++) {
//    std::cout << "\n************************************************** " << std::endl;
//    std::cout << "*                   RUN  # " << i << "                     * " << std::endl;
//    std::cout << "************************************************** \n"
//              << std::endl;
//
//
//
//
//
//    Alien::Ginkgo::LinearAlgebra algebra;
//    // compute explicit residual r = ||Ax - b|| ~ 0
//    Alien::Move::VectorData r(A.rowSpace(), A.distribution().rowDistribution());
//    algebra.mult(A, x, r);
//    algebra.axpy(-1., b, r);
//    auto norm_r = algebra.norm2(r);
//    auto norm_b = algebra.norm2(b);
//    tm->info() << " => ||Ax-b|| = " << norm_r;
//    tm->info() << " => ||b|| = " << norm_b;
//    tm->info() << " => ||Ax-b||/||b|| = " << norm_r / norm_b;
//
//    /* Check results :
//     * min(x), max(x), min|x|, max|x|
//     * err_max : ||Ax-b||_{inf}
//     * rerr_max :||Ax-b||_{inf} / ||b|| _{inf}
//     */
//
//    std::cout << "max(x) : " << vecMax(x) << std::endl;
//    std::cout << "min(x) : " << vecMin(x) << std::endl;
//    std::cout << "maxAbs(x) : " << vecMaxAbs(x) << std::endl;
//    std::cout << "minAbs(x) : " << vecMinAbs(x) << std::endl;
//    std::cout << "max_error : " << vecMaxAbs(r) << std::endl;
//    // std::cout << "absmaxB(b) : " << vecMaxAbs(b) << std::endl;
//    std::cout << "rmax_error : " << vecMaxAbs(r) / vecMaxAbs(b) << std::endl;
//    std::cout << "=================================== " << std::endl;
//  }

  return 0;
}

int main(int argc, char** argv)
{

  MPI_Init(&argc, &argv);

  if (argc < 4) {
    std::cerr << "Usage : ./bench_ginkgo [solver] [preconditioner] [matrix] [vector] \n"
              << "  - solver : (CG|GMRES|BICG|BICGSTAB) \n"
              << "  - preconditioner : (Jacobi|NoPC) \n"
              << "  - MTX matrix file \n"
              << "  - optional MTX vector file \n";
    return -1;
  }

  // Read the solver
  Alien::Ginkgo::OptionTypes::eSolver solver;
  if (std::string(argv[1]) == "CG") {
    solver = Alien::Ginkgo::OptionTypes::CG;
  }
  else if (std::string(argv[1]) == "GMRES") {
    solver = Alien::Ginkgo::OptionTypes::GMRES;
  }
  else if (std::string(argv[1]) == "BICG") {
    solver = Alien::Ginkgo::OptionTypes::BICG;
  }
  else if (std::string(argv[1]) == "BICGSTAB") {
    solver = Alien::Ginkgo::OptionTypes::BICGSTAB;
  }
  else {
    std::cerr << "Unrecognized solver : " << argv[1] << "\n"
              << "  - solver list : (CG|GMRES|BICG|BICGSTAB) \n";
    return -1;
  }

  // Read the preconditioner
  Alien::Ginkgo::OptionTypes::ePreconditioner prec;
  if (std::string(argv[2]) == "Jacobi") {
    prec = Alien::Ginkgo::OptionTypes::Jacobi;
  }
  else if (std::string(argv[2]) == "NoPC") {
    prec = Alien::Ginkgo::OptionTypes::NoPC;
  }
  else {
    std::cerr << "Unrecognized preconditioner : " << argv[2] << "\n"
              << "  - preconditioner list : (Jacobi|NoPC) \n";
    return -1;
  }

  // Read Matrix file
  std::string matrix_file;
  // Read matrix file
  if (argv[3]) {
    matrix_file = std::string(argv[3]);
  }
  else {
    std::cerr << "Matrix File is needed for this bench.";
    return -1;
  }

  // Read optional Vector file
  std::string vec_file = "";
  if (argv[4]) {
    vec_file = std::string(argv[4]);
  }

  auto ret = 0;
  try {
    ret = test(solver, prec, matrix_file, vec_file);
  }
  catch (const Arccore::Exception& ex) {
    std::cerr << "Exception: " << ex << '\n';
    ret = 3;
  }
  catch (const std::exception& ex) {
    std::cerr << "** A standard exception occured: " << ex.what() << ".\n";
    ret = 2;
  }
  catch (...) {
    std::cerr << "** An unknown exception has occured...\n";
    ret = 1;
  }

  MPI_Finalize();
  return ret;
}