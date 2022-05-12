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

#include "trilinos_linear_solver.h"

namespace Alien
{
// Compile TrilinosLinearSolver.
template class ALIEN_TRILINOS_EXPORT LinearSolver<BackEnd::tag::trilinos>;
} // namespace Alien

namespace Alien::Trilinos
{

bool InternalLinearSolver::solve(const Matrix& A, const Vector& b, Vector& x)
{
  using Teuchos::ParameterList;
  using Teuchos::RCP;
  using Teuchos::rcp;

  /*
  // Create Belos iterative linear solver.
  RCP<solver_type> solver;
  RCP<ParameterList> solverParams(new ParameterList());
  {
    solverParams->set("Maximum Iterations", 400); //m_options.numIterationsMax()
    solverParams->set("Convergence Tolerance", 1.0e-8); //m_options.stopCriteriaValue()
    solverParams->set( "Verbosity", Belos::Errors + Belos::Warnings + Belos::TimingDetails + Belos::FinalSummary + Belos::StatusTestDetails );

    Belos::SolverFactory<SC, MV, OP> belosFactory;
    solver = belosFactory.create("cg", solverParams);
  }
  if (solver.is_null()) {
    return -1;
  }
*/

  // Create Ifpack2 preconditioner.
  Teuchos::RCP<prec_type> M;
  {
    M = Ifpack2::Factory::create<row_matrix_type>("RELAXATION", A.internal());
    if (M.is_null()) {
      std::cerr << "Failed to create Ifpack2 preconditioner !" << std::endl;
      return -1;
    }
    M->initialize();
    M->compute();
  }

  // Create a preconditioned linear problem instance.
  Belos::LinearProblem<SC, MV, OP> problem(A.internal(), x.internal(), b.internal());
  problem.setRightPrec(M);
  bool set = problem.setProblem();
  if (set == false) {
    std::cout << std::endl
              << "ERROR:  Belos::LinearProblem failed to set up correctly !" << std::endl;
    return -1;
  }

  // Create the solver
  ParameterList belosList;
  belosList.set("Maximum Iterations", m_options.numIterationsMax());
  belosList.set("Convergence Tolerance", m_options.stopCriteriaValue());
  Belos::BlockCGSolMgr<SC, MV, OP> solver(rcpFromRef(problem), rcpFromRef(belosList));

  // Add Logger !
  //Belos::StatusTestLogResNorm<SC, MV, OP> debugTest = Belos::StatusTestLogResNorm<SC, MV, OP>(m_options.numIterationsMax());
  //solver.setDebugStatusTest( Teuchos::rcp(&debugTest, false) );


  // Init timer
  std::chrono::nanoseconds time(0);

  // Run !
  auto tic = std::chrono::steady_clock::now();
  Belos::ReturnType ret = solver.solve();
  auto toc = std::chrono::steady_clock::now();
  time += std::chrono::duration_cast<std::chrono::nanoseconds>(toc - tic);


    // Check
    if (ret == Belos::Converged) {
      std::cout << "Belos Solver has converged." << std::endl;

      // Get solver infos
      const int numIters = solver.getNumIters();
      std::cout << "numIters : " << numIters << std::endl;
      std::cout << "achieved tol : " << solver.achievedTol() << std::endl;

      // Print timing infos
      double ms = static_cast<double>(time.count()) / 1e6;
      double sec = static_cast<double>(time.count()) / 1e9;
      double it_per_sec = numIters / sec;
      std::cout << "Execution time [ms]: " << ms << std::endl;
      std::cout << "Execution time [s]: " << sec << std::endl;
      std::cout << "Iterations per second : " << it_per_sec << std::endl;


      /*std::cout << "Residual norm : " << residual_norm << std::endl;*/


      // update solver status
      /*m_status.residual = residual_norm;*/
      m_status.iteration_count = numIters;
      m_status.succeeded = true;

      // update solver infos
      m_total_iter_num += m_status.iteration_count;
      ++m_solve_num;
      /*m_total_solve_time += sec;*/
    }
    else if (ret == Belos::Unconverged) {
      m_status.succeeded = false;
      std::cout << "Belos Solver did not converge !" << std::endl;
    }

  return m_status.succeeded;
}

const Alien::SolverStatus&
InternalLinearSolver::getStatus() const
{
  return m_status;
}

ALIEN_TRILINOS_EXPORT
std::shared_ptr<ILinearAlgebra>
InternalLinearSolver::algebra() const
{
  return std::make_shared<LinearAlgebra>();
}

ALIEN_TRILINOS_EXPORT
IInternalLinearSolver<Matrix, Vector>*
InternalLinearSolverFactory(const Options& options)
{
  return new InternalLinearSolver(options);
}

ALIEN_TRILINOS_EXPORT
IInternalLinearSolver<Matrix, Vector>*
InternalLinearSolverFactory()
{
  return new InternalLinearSolver();
}
} // namespace Alien::Trilinos
