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

#include "matrix.h"
#include "vector.h"
#include "ginkgo_logger.h"

#include <boost/timer.hpp>

#include <arccore/message_passing_mpi/MpiMessagePassingMng.h>

#include <alien/expression/solver/SolverStater.h>
#include <alien/core/backend/LinearSolverT.h>

#include <alien/ginkgo/backend.h>
#include <alien/ginkgo/options.h>
#include <alien/ginkgo/export.h>

#include <core/solver/cg.hpp>
#include <core/stop/iteration.hpp>
#include <core/stop/residual_norm.hpp>

#include <memory>
#include <core/log/convergence.hpp>

namespace Alien
{
// Compile GinkgoLinearSolver.
template class ALIEN_GINKGO_EXPORT LinearSolver<BackEnd::tag::ginkgo>;

} // namespace Alien

namespace Alien::Ginkgo
{
class InternalLinearSolver
: public IInternalLinearSolver<Matrix, Vector>
, public ObjectWithTrace
{
 public:
  typedef SolverStatus Status;
  InternalLinearSolver();
  explicit InternalLinearSolver(const Options& options);
  ~InternalLinearSolver() override = default;

 public:
  // Nothing to do
  void updateParallelMng(
  Arccore::MessagePassing::IMessagePassingMng* pm) override {}

  bool solve(const Matrix& A, const Vector& b, Vector& x) override;
  bool hasParallelSupport() const override { return true; }

  //! Etat du solveur
  const Status& getStatus() const override;
  const SolverStat& getSolverStat() const override { return m_stat; }

  std::shared_ptr<ILinearAlgebra> algebra() const override;

 private:
  Status m_status;
  Arccore::Real m_init_time{ 0.0 };
  Arccore::Real m_total_solve_time{ 0.0 };
  Arccore::Integer m_solve_num{ 0 };
  Arccore::Integer m_total_iter_num{ 0 };

  SolverStat m_stat;
  Options m_options;

 private:
  using stop_iter_type = std::unique_ptr<gko::stop::Iteration::Factory, std::default_delete<gko::stop::Iteration::Factory>>;
  using stop_res_type = std::unique_ptr<gko::stop::AbsoluteResidualNorm<>::Factory, std::default_delete<gko::stop::AbsoluteResidualNorm<>::Factory>>;
  using exec_type = std::shared_ptr<const gko::Executor>;

  void solve_CG(const Matrix& A, const Vector& b, Vector& x, const int& prec, stop_iter_type& iter_stop, stop_res_type& res_stop, exec_type& exec, std::chrono::nanoseconds& time);
  void solve_GMRES(const Matrix& A, const Vector& b, Vector& x, const int& prec, stop_iter_type& iter_stop, stop_res_type& res_stop, exec_type& exec, std::chrono::nanoseconds& time);
  void solve_BICG(const Matrix& A, const Vector& b, Vector& x, const int& prec, stop_iter_type& iter_stop, stop_res_type& res_stop, exec_type& exec, std::chrono::nanoseconds& time);
  void solve_BICGSTAB(const Matrix& A, const Vector& b, Vector& x, const int& prec, stop_iter_type& iter_stop, stop_res_type& res_stop, exec_type& exec, std::chrono::nanoseconds& time);
  void display_solver_infos(const Alien::Ginkgo::OptionTypes::eSolver& solver, const Alien::Ginkgo::OptionTypes::ePreconditioner& prec);
};

InternalLinearSolver::InternalLinearSolver()
{
  boost::timer tinit;
  m_init_time += tinit.elapsed();
}

InternalLinearSolver::InternalLinearSolver(const Options& options)
: m_options(options)
{
  boost::timer tinit;
  m_init_time += tinit.elapsed();
}

bool InternalLinearSolver::solve(const Matrix& A, const Vector& b, Vector& x)
{
  // Macro "pratique" en attendant de trouver mieux
  // boost::timer tsolve;

  // preconditioner's choice
  int prec;
  switch (m_options.preconditioner()) {
  case OptionTypes::Jacobi:
    prec = 1;
    break;
  case OptionTypes::NoPC:
    prec = 0;
    break;
  default:
    alien_fatal([&] { cout() << "Undefined Ginkgo preconditioner option"; });
    break;
  }

  // get the executor from the Matrix A
  auto exec = A.internal()->get_executor();

  // Prepare the stopping criteria
  const double threshold{ m_options.stopCriteriaValue() };
  const unsigned long max_iters = static_cast<unsigned int>(m_options.numIterationsMax());

  auto iter_stop = gko::stop::Iteration::build()
                   .with_max_iters(max_iters)
                   .on(exec);

  //auto res_stop = gko::stop::RelativeResidualNorm<>::build() // relative (to ||b||) norm
  auto res_stop = gko::stop::AbsoluteResidualNorm<>::build() // absolute norm
                  .with_tolerance(threshold)
                  .on(exec);

  // Add Convergence logger
  std::shared_ptr<const gko::log::Convergence<double>> conv_logger = gko::log::Convergence<double>::create(exec);
  iter_stop->add_logger(conv_logger);
  res_stop->add_logger(conv_logger);

  // init timer
  std::chrono::nanoseconds time(0);

  /// --- Find a better way to switch solver ...
  switch (m_options.solver()) {
  case OptionTypes::CG:
    solve_CG(A, b, x, prec, iter_stop, res_stop, exec, time);
    break;
  case OptionTypes::GMRES:
    solve_GMRES(A, b, x, prec, iter_stop, res_stop, exec, time);
    break;
  case OptionTypes::BICG:
    solve_BICG(A, b, x, prec, iter_stop, res_stop, exec, time);
    break;
  case OptionTypes::BICGSTAB:
    solve_BICGSTAB(A, b, x, prec, iter_stop, res_stop, exec, time);
    break;
  default:
    alien_fatal([&] {
      cout() << "Undefined solver option";
    });
    break;
  }

  // Get nb iterations + final residual
  auto num_iters = conv_logger->get_num_iterations();
  auto residual_norm = conv_logger->get_residual_norm();
  auto vec_res_norm = reinterpret_cast<const gko::matrix::Dense<double>*>(residual_norm);
  auto res = vec_res_norm->get_const_values()[0];

  // Print infos
  std::cout << "===== SOLVER  RUN INFORMATION ===== " << std::endl;
  display_solver_infos(m_options.solver(), m_options.preconditioner());
  std::cout << "Stop criteria Value : " << m_options.stopCriteriaValue() << std::endl;
  std::cout << "Solver has converged : " << conv_logger->has_converged() << std::endl;
  std::cout << "Nb iterations : " << num_iters << std::endl;
  std::cout << "Residual norm : " << res << std::endl;
  std::cout << "Execution time [ms]: " << static_cast<double>(time.count()) / 1000000.0 << std::endl;
  std::cout << "Time per iteration [ms]: " << static_cast<double>(time.count()) / 1000000.0 / num_iters << std::endl;
  std::cout << "Iterations per second : " << num_iters / (static_cast<double>(time.count()) / 1000000.0) << std::endl;
  std::cout << "=================================== " << std::endl;

  // update the counters
  m_total_iter_num += num_iters;
  ++m_solve_num;
  //m_total_solve_time += tsolve.elapsed();
}

void InternalLinearSolver::solve_CG(const Matrix& A, const Vector& b, Vector& x, const int& prec, stop_iter_type& iter_stop, stop_res_type& res_stop, exec_type& exec, std::chrono::nanoseconds& time)
{

  auto solver_factory =
  gko::solver::Cg<double>::build()
  .with_criteria(
  gko::share(iter_stop),
  gko::share(res_stop))
  .on(exec);

  // make a shared pointer on the matrix A, make_shared does not work : loses the pointer to the gko::executor !
  auto pA = std::shared_ptr<const gko::matrix::Csr<double, int>>(A.internal(), [](auto* p) { std::cout << " Did not delete the shared_pointer ! " << std::endl; });

  // generate the solver
  auto solver = solver_factory->generate(pA);

  // generate and set the preconditioner
  if (prec == 1) {
    using Preconditioner = typename gko::preconditioner::Jacobi<double>;
    std::shared_ptr<Preconditioner> new_prec = Preconditioner::build().on(exec)->generate(pA);
    solver->set_preconditioner(new_prec);
  }

  // solve with timing
  auto tic = std::chrono::steady_clock::now();
  solver->apply(lend(b.internal()), lend(x.internal()));
  auto toc = std::chrono::steady_clock::now();
  time += std::chrono::duration_cast<std::chrono::nanoseconds>(toc - tic);
}

void InternalLinearSolver::solve_GMRES(const Matrix& A, const Vector& b, Vector& x, const int& prec, stop_iter_type& iter_stop, stop_res_type& res_stop, exec_type& exec, std::chrono::nanoseconds& time)
{

  auto solver_factory =
  gko::solver::Gmres<double>::build()
  .with_criteria(
  gko::share(iter_stop),
  gko::share(res_stop))
  .on(exec);

  // make a shared pointer on the matrix A, make_shared does not work : loses the pointer to the gko::executor !
  auto pA = std::shared_ptr<const gko::matrix::Csr<double, int>>(A.internal(), [](auto* p) { std::cout << " Did not delete the shared_pointer ! " << std::endl; });

  // generate the solver
  auto solver = solver_factory->generate(pA);

  // generate and set the preconditioner
  if (prec == 1) {
    using Preconditioner = typename gko::preconditioner::Jacobi<double>;
    std::shared_ptr<Preconditioner> new_prec = Preconditioner::build().on(exec)->generate(pA);
    solver->set_preconditioner(new_prec);
  }

  // solve with timing
  auto tic = std::chrono::steady_clock::now();
  solver->apply(lend(b.internal()), lend(x.internal()));
  auto toc = std::chrono::steady_clock::now();
  time += std::chrono::duration_cast<std::chrono::nanoseconds>(toc - tic);
}

void InternalLinearSolver::solve_BICG(const Matrix& A, const Vector& b, Vector& x, const int& prec, stop_iter_type& iter_stop, stop_res_type& res_stop, exec_type& exec, std::chrono::nanoseconds& time)
{

  auto solver_factory =
  gko::solver::Bicg<double>::build()
  .with_criteria(
  gko::share(iter_stop),
  gko::share(res_stop))
  .on(exec);

  // make a shared pointer on the matrix A, make_shared does not work : loses the pointer to the gko::executor !
  auto pA = std::shared_ptr<const gko::matrix::Csr<double, int>>(A.internal(), [](auto* p) { std::cout << " Did not delete the shared_pointer ! " << std::endl; });

  // generate the solver
  auto solver = solver_factory->generate(pA);

  // generate and set the preconditioner
  if (prec == 1) {
    using Preconditioner = typename gko::preconditioner::Jacobi<double>;
    std::shared_ptr<Preconditioner> new_prec = Preconditioner::build().on(exec)->generate(pA);
    solver->set_preconditioner(new_prec);
  }

  // solve with timing
  auto tic = std::chrono::steady_clock::now();
  solver->apply(lend(b.internal()), lend(x.internal()));
  auto toc = std::chrono::steady_clock::now();
  time += std::chrono::duration_cast<std::chrono::nanoseconds>(toc - tic);
}

void InternalLinearSolver::solve_BICGSTAB(const Matrix& A, const Vector& b, Vector& x, const int& prec, stop_iter_type& iter_stop, stop_res_type& res_stop, exec_type& exec, std::chrono::nanoseconds& time)
{
  auto solver_factory =
  gko::solver::Bicgstab<double>::build()
  .with_criteria(
  gko::share(iter_stop),
  gko::share(res_stop))
  .on(exec);

  // make a shared pointer on the matrix A, make_shared does not work : loses the pointer to the gko::executor !
  auto pA = std::shared_ptr<const gko::matrix::Csr<double, int>>(A.internal(), [](auto* p) { std::cout << " Did not delete the shared_pointer ! " << std::endl; });

  // generate the solver
  auto solver = solver_factory->generate(pA);

  // generate and set the preconditioner
  if (prec == 1) {
    using Preconditioner = typename gko::preconditioner::Jacobi<double>;
    std::shared_ptr<Preconditioner> new_prec = Preconditioner::build().on(exec)->generate(pA);
    solver->set_preconditioner(new_prec);
  }

  // solve with timing
  auto tic = std::chrono::steady_clock::now();
  solver->apply(lend(b.internal()), lend(x.internal()));
  auto toc = std::chrono::steady_clock::now();
  time += std::chrono::duration_cast<std::chrono::nanoseconds>(toc - tic);
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
