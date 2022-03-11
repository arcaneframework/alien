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

#include "ginkgo_linear_solver.h"

bool InternalLinearSolver::solve(const Matrix& A, const Vector& b, Vector& x)
{

  // get the executor from the Matrix A
  auto exec = A.internal()->get_executor();

  // make a shared pointer on the matrix A
  auto pA = std::shared_ptr<const gko::matrix::Csr<double, int>>(A.internal(), [](auto* p) {});

  /**
   handle preconditioner choice
  **/

  // use LinOpFactory for factorizing
  std::shared_ptr<gko::LinOpFactory> prec_factory;

  // for ilu, generate concrete factorization for input matrix
  auto par_ilu_fact = gko::factorization::ParIlu<double>::build().on(exec);
  auto par_ilu = par_ilu_fact->generate(pA);

  int prec;
  switch (m_options.preconditioner()) {
  case OptionTypes::Ilu:
    prec = 2;
    // Generate an ILU preconditioner factory by setting lower and upper triangular solver
    // - in this case the exact triangular solves
    prec_factory = gko::preconditioner::Ilu<
                   gko::solver::LowerTrs<double>,
                   gko::solver::UpperTrs<double>,
                   false>::build()
                   .on(exec);
    break;
  case OptionTypes::Jacobi:
    prec = 1;
    prec_factory = gko::preconditioner::Jacobi<double>::build().on(exec);
    break;
  case OptionTypes::NoPC:
    prec = 0;
    break;
  default:
    alien_fatal([&] { cout() << "Undefined Ginkgo preconditioner option"; });
    break;
  }

  /**
   Prepare the stopping criteria
  **/

  const double threshold{ m_options.stopCriteriaValue() };
  const unsigned long max_iters = static_cast<unsigned int>(m_options.numIterationsMax());

  auto iter_stop = gko::stop::Iteration::build()
                   .with_max_iters(max_iters)
                   .on(exec);

  auto res_stop = gko::stop::RelativeResidualNorm<>::build() // relative (to ||b||) norm
                  //auto res_stop = gko::stop::AbsoluteResidualNorm<>::build() // absolute norm
                  .with_tolerance(threshold)
                  .on(exec);

  /**
   Add Convergence logger
  **/

  std::shared_ptr<const gko::log::Convergence<double>> conv_logger = gko::log::Convergence<double>::create(exec);
  iter_stop->add_logger(conv_logger);
  res_stop->add_logger(conv_logger);

  /**
   handle solver choice
  **/

  // Factorize with LinOpFactory and LinOp
  std::shared_ptr<gko::LinOpFactory> solver_factory;
  std::shared_ptr<gko::LinOp> solver;

  switch (m_options.solver()) {
  case OptionTypes::CG: {
    auto solver_factory =
    gko::solver::Cg<double>::build()
    .with_criteria(
    gko::share(iter_stop),
    gko::share(res_stop))
    .on(exec);

    // generate the solver
    solver = solver_factory->generate(pA);

    // set the preconditioner
    if (prec == 1) { //Jacobi
      gko::as<gko::solver::Cg<double>>(solver)->set_preconditioner(prec_factory->generate(pA));
    }
    else if (prec == 2) { //ILU
      gko::as<gko::solver::Cg<double>>(solver)->set_preconditioner(prec_factory->generate(gko::share(par_ilu)));
    }

  } break;
  case OptionTypes::GMRES: {
    auto solver_factory =
    gko::solver::Gmres<double>::build()
    .with_criteria(
    gko::share(iter_stop),
    gko::share(res_stop))
    .on(exec);

    // generate the solver
    solver = solver_factory->generate(pA);

    // set the preconditioner
    if (prec == 1) { //Jacobi
      gko::as<gko::solver::Gmres<double>>(solver)->set_preconditioner(prec_factory->generate(pA));
    }
    else if (prec == 2) { //ILU
      gko::as<gko::solver::Gmres<double>>(solver)->set_preconditioner(prec_factory->generate(gko::share(par_ilu)));
    }
  } break;
  case OptionTypes::BICG: {
    auto solver_factory =
    gko::solver::Bicg<double>::build()
    .with_criteria(
    gko::share(iter_stop),
    gko::share(res_stop))
    .on(exec);

    // generate the solver
    solver = solver_factory->generate(pA);

    // set the preconditioner
    if (prec == 1) { //Jacobi
      gko::as<gko::solver::Bicg<double>>(solver)->set_preconditioner(prec_factory->generate(pA));
    }
    else if (prec == 2) { //ILU
      gko::as<gko::solver::Bicg<double>>(solver)->set_preconditioner(prec_factory->generate(gko::share(par_ilu)));
    }
  } break;
  case OptionTypes::BICGSTAB: {
    auto solver_factory =
    gko::solver::Bicgstab<double>::build()
    .with_criteria(
    gko::share(iter_stop),
    gko::share(res_stop))
    .on(exec);

    // generate the solver
    solver = solver_factory->generate(pA);

    // set the preconditioner
    if (prec == 1) { //Jacobi
      gko::as<gko::solver::Bicgstab<double>>(solver)->set_preconditioner(prec_factory->generate(pA));
    }
    else if (prec == 2) { //ILU
      gko::as<gko::solver::Bicgstab<double>>(solver)->set_preconditioner(prec_factory->generate(gko::share(par_ilu)));
    }
  } break;
  default:
    alien_fatal([&] {
      cout() << "Undefined solver option";
    });
    break;
  }

  /**
   solve with timing
  **/

  std::chrono::nanoseconds time(0);
  auto tic = std::chrono::steady_clock::now();
  solver->apply(lend(b.internal()), lend(x.internal()));
  auto toc = std::chrono::steady_clock::now();
  time += std::chrono::duration_cast<std::chrono::nanoseconds>(toc - tic);

  /**
   Update infos
  **/

  // Get nb iterations + final residual
  auto num_iters = conv_logger->get_num_iterations();
  auto vec_res_norm = reinterpret_cast<const gko::matrix::Dense<double>*>(conv_logger->get_residual_norm());
  auto residual_norm = vec_res_norm->get_const_values()[0];

  // Print results infos
  std::cout << "===== SOLVER  RUN INFORMATION ===== " << std::endl;
  std::cout << "Ginkgo Executor : " << Alien::Ginkgo::Ginkgo_executor::get_target_machine() << std::endl;
  display_solver_infos(m_options.solver(), m_options.preconditioner());
  std::cout << "Stop criteria Value : " << m_options.stopCriteriaValue() << std::endl;
  std::cout << "Solver has converged : " << conv_logger->has_converged() << std::endl;
  std::cout << "Nb iterations : " << num_iters << std::endl;
  std::cout << "Residual norm : " << residual_norm << std::endl;
  auto norm_b = gko::initialize<gko::matrix::Dense<double>>({ 0.0 }, exec);
  b.internal()->compute_norm2(gko::lend(norm_b));
  std::cout << "Convergence : " << residual_norm / (norm_b->get_const_values()[0]) << std::endl; // Only for relative residual

  // Print timing infos
  double ms = static_cast<double>(time.count()) / 1e6;
  double sec = static_cast<double>(time.count()) / 1e9;
  double it_per_sec = num_iters / sec;
  std::cout << "Execution time [ms]: " << ms << std::endl;
  std::cout << "Execution time [s]: " << sec << std::endl;
  std::cout << "Iterations per second : " << it_per_sec << std::endl;

  // update solver status
  m_status.residual = residual_norm;
  m_status.iteration_count = num_iters;
  m_status.succeeded = conv_logger->has_converged();

  // update solver infos
  m_total_iter_num += m_status.iteration_count;
  ++m_solve_num;
  m_total_solve_time += sec;

  return m_status.succeeded;
}

void InternalLinearSolver::display_solver_infos(const Alien::Ginkgo::OptionTypes::eSolver& solver, const Alien::Ginkgo::OptionTypes::ePreconditioner& prec)
{
  std::cout << "Solver : ";
  switch (solver) {
  case OptionTypes::CG:
    std::cout << "CG" << std::endl;
    break;
  case OptionTypes::GMRES:
    std::cout << "GMRES" << std::endl;
    break;
  case OptionTypes::BICG:
    std::cout << "BICG" << std::endl;
    break;
  case OptionTypes::BICGSTAB:
    std::cout << "BICGSTAB" << std::endl;
    break;
  default:
    std::cout << "undefined !" << std::endl;
    break;
  }

  std::cout << "Preconditioner : ";
  switch (prec) {
  case OptionTypes::Jacobi:
    std::cout << "Jacobi" << std::endl;
    break;
  case OptionTypes::Ilu:
    std::cout << "Ilu" << std::endl;
    break;
  case OptionTypes::NoPC:
    std::cout << "No preconditioner" << std::endl;
    break;
  default:
    std::cout << "undefined !" << std::endl;
    break;
  }
}

const Alien::SolverStatus&
InternalLinearSolver::getStatus() const
{
  return m_status;
}

ALIEN_GINKGO_EXPORT
std::shared_ptr<ILinearAlgebra>
InternalLinearSolver::algebra() const
{
  return std::make_shared<LinearAlgebra>();
}

ALIEN_GINKGO_EXPORT
IInternalLinearSolver<Matrix, Vector>*
InternalLinearSolverFactory(const Options& options)
{
  return new InternalLinearSolver(options);
}

ALIEN_GINKGO_EXPORT
IInternalLinearSolver<Matrix, Vector>*
InternalLinearSolverFactory()
{
  return new InternalLinearSolver();
}
} // namespace Alien::Ginkgo
