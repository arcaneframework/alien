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
  void checkError(const Arccore::String& msg, int ierr,
                  int skipError = 0) const;
  void solve_GMRES(const Matrix& A, const Vector& b, Vector& x, auto& iter_stop, auto& res_stop, auto& exec, auto& conv_logger, const int& output_level, std::chrono::nanoseconds & time);
  void solve_CG(const Matrix& A, const Vector& b, Vector& x, auto& iter_stop, auto& res_stop, auto& exec, auto& conv_logger, const int& output_level, std::chrono::nanoseconds & time);
  void solve_BICG(const Matrix& A, const Vector& b, Vector& x, auto& iter_stop, auto& res_stop, auto& exec, auto& conv_logger, const int& output_level, std::chrono::nanoseconds & time);
  void solve_BICGSTAB(const Matrix& A, const Vector& b, Vector& x, auto& iter_stop, auto& res_stop, auto& exec, auto& conv_logger, const int& output_level, std::chrono::nanoseconds & time);
  void solve_GMRES_jacobi(const Matrix& A, const Vector& b, Vector& x, auto& iter_stop, auto& res_stop, auto& exec, auto& conv_logger, const int& output_level, std::chrono::nanoseconds & time);
  void solve_CG_jacobi(const Matrix& A, const Vector& b, Vector& x, auto& iter_stop, auto& res_stop, auto& exec, auto& conv_logger, const int& output_level, std::chrono::nanoseconds & time);
  void solve_BICG_jacobi(const Matrix& A, const Vector& b, Vector& x, auto& iter_stop, auto& res_stop, auto& exec, auto& conv_logger, const int& output_level, std::chrono::nanoseconds & time);
  void solve_BICGSTAB_jacobi(const Matrix& A, const Vector& b, Vector& x, auto& iter_stop, auto& res_stop, auto& exec, auto& conv_logger, const int& output_level, std::chrono::nanoseconds & time);
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

void InternalLinearSolver::checkError(const Arccore::String& msg, int ierr, int skipError) const
{
  if (ierr != 0 and (ierr & ~skipError) != 0) {
    alien_fatal([&] {
      cout() << msg << " failed. [code=" << ierr << "]";
      //CHKERRQ(ierr); // warning car macro qui appelle fx qui retourne un int
    });
  }
}

bool InternalLinearSolver::solve(const Matrix& A, const Vector& b, Vector& x)
{
  // Macro "pratique" en attendant de trouver mieux
  boost::timer tsolve;

  int output_level = m_options.verbose() ? 1 : 0;

  /// --- Find a better way to switch preconditioner ...
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

  // Prepare Convergence logger
  std::shared_ptr<const gko::log::Convergence<double>> conv_logger = gko::log::Convergence<double>::create(exec);
  iter_stop->add_logger(conv_logger);
  res_stop->add_logger(conv_logger);

  // init timer
  std::chrono::nanoseconds time(0);

  /// --- Find a better way to switch solver ...
  switch (m_options.solver()) {
  case OptionTypes::GMRES:
    if (prec == 1)
      solve_GMRES_jacobi(A, b, x, iter_stop, res_stop, exec, conv_logger, output_level, time);
    else
      solve_GMRES(A, b, x, iter_stop, res_stop, exec, conv_logger, output_level, time);
    break;
  case OptionTypes::CG:
    if (prec == 1)
      solve_CG_jacobi(A, b, x, iter_stop, res_stop, exec, conv_logger, output_level, time);
    else
      solve_CG(A, b, x, iter_stop, res_stop, exec, conv_logger, output_level, time);
    break;
  case OptionTypes::BICG:
    if (prec == 1)
      solve_BICG_jacobi(A, b, x, iter_stop, res_stop, exec, conv_logger, output_level, time);
    else
      solve_BICG(A, b, x, iter_stop, res_stop, exec, conv_logger, output_level, time);

    break;
  case OptionTypes::BICGSTAB:
    if (prec == 1)
      solve_BICGSTAB_jacobi(A, b, x, iter_stop, res_stop, exec, conv_logger, output_level, time);
    else
      solve_BICGSTAB(A, b, x, iter_stop, res_stop, exec, conv_logger, output_level, time);
    break;
  default:
    alien_fatal([&] {
      cout() << "Undefined solver option";
    });
    break;
  }

  // get nb iterations + final residual
  auto num_iters = conv_logger->get_num_iterations();
  auto residual_norm = conv_logger->get_residual_norm();
  auto vec_res_norm = reinterpret_cast<const gko::matrix::Dense<double>*>(residual_norm);
  auto res = vec_res_norm->get_const_values()[0];

  // Print infos
  std::cout << "===== SOLVER  RUN INFORMATION ===== "<< std::endl;
  std::cout << "Stop criteria Value : " << m_options.stopCriteriaValue() << std::endl;
  std::cout << "Solver has converged : " << conv_logger->has_converged() << std::endl;
  std::cout << "Nb iterations : " << num_iters << std::endl;
  std::cout << "Residual norm : " << res << std::endl;
  std::cout << "Execution time [ms]: " << static_cast<double>(time.count()) / 1000000.0 << std::endl;
  std::cout << "Time per iteration [ms]: " << static_cast<double>(time.count()) / 1000000.0 / num_iters  << std::endl;
  std::cout << "=================================== "<< std::endl;


  // update the counters
  m_total_iter_num += num_iters;
  ++m_solve_num;
  m_total_solve_time += tsolve.elapsed();
}

void InternalLinearSolver::solve_CG_jacobi(const Matrix& A, const Vector& b, Vector& x, auto& iter_stop, auto& res_stop, auto& exec, auto& conv_logger, const int& output_level, std::chrono::nanoseconds & time)
{
  std::cout << "----------- Solve with CG + Jacobi : " << std::endl;

  auto solver_factory =
  gko::solver::Cg<double>::build()
  .with_preconditioner(
  gko::preconditioner::Jacobi<>::build()
  .on(exec))
  .with_criteria(
  gko::share(iter_stop),
  gko::share(res_stop))
  .on(exec);

  // make a shared pointer on the matrix A
  /* make_shared does not work : loses the pointer to the gko::executor ! */
  auto pA = std::shared_ptr<const gko::matrix::Csr<double, int>>(A.internal(), [](auto* p) { std::cout << " Did not delete the shared_pointer ! " << std::endl; });

  // Instantiate a ResidualLogger logger.
  auto res_logger = std::make_shared<ResidualLogger<double>>(exec, gko::lend(pA), gko::lend(b.internal()));

  if (output_level) {
    // Add the previously created logger to the solver factory.
    solver_factory->add_logger(res_logger);
  }

  // generate the solver
  auto cg_solver = solver_factory->generate(share(pA));

  // solve with timing
  auto tic = std::chrono::steady_clock::now();
  cg_solver->apply(lend(b.internal()), lend(x.internal()));
  auto toc = std::chrono::steady_clock::now();
  time += std::chrono::duration_cast<std::chrono::nanoseconds>(toc - tic);

  if (output_level) {
    // Print the table of the residuals obtained from the logger
    res_logger->write();
  }
}

void InternalLinearSolver::solve_GMRES_jacobi(const Matrix& A, const Vector& b, Vector& x, auto& iter_stop, auto& res_stop, auto& exec, auto& conv_logger, const int& output_level, std::chrono::nanoseconds & time)
{
  std::cout << "----------- Solve with GMRES + Jacobi : " << std::endl;

  auto solver_factory =
  gko::solver::Gmres<double>::build()
  .with_preconditioner(
  gko::preconditioner::Jacobi<>::build()
  .on(exec))
  .with_criteria(
  gko::share(iter_stop),
  gko::share(res_stop))
  .on(exec);

  // make a shared pointer on the matrix A
  /* make_shared does not work : loses the pointer to the gko::executor ! */
  auto pA = std::shared_ptr<const gko::matrix::Csr<double, int>>(A.internal(), [](auto* p) { std::cout << " Did not delete the shared_pointer ! " << std::endl; });

  // Instantiate a ResidualLogger logger.
  auto res_logger = std::make_shared<ResidualLogger<double>>(exec, gko::lend(pA), gko::lend(b.internal()));

  if (output_level) {
    // Add the previously created logger to the solver factory.
    solver_factory->add_logger(res_logger);
  }

  // generate the solver
  auto cg_solver = solver_factory->generate(share(pA));

  // solve with timing
  auto tic = std::chrono::steady_clock::now();
  cg_solver->apply(lend(b.internal()), lend(x.internal()));
  auto toc = std::chrono::steady_clock::now();
  time += std::chrono::duration_cast<std::chrono::nanoseconds>(toc - tic);

  if (output_level) {
    // Print the table of the residuals obtained from the logger
    res_logger->write();
  }
}

void InternalLinearSolver::solve_BICG_jacobi(const Matrix& A, const Vector& b, Vector& x, auto& iter_stop, auto& res_stop, auto& exec, auto& conv_logger, const int& output_level, std::chrono::nanoseconds & time)
{
  std::cout << "----------- Solve with BICG + Jacobi : " << std::endl;

  auto solver_factory =
  gko::solver::Bicg<double>::build()
  .with_preconditioner(
  gko::preconditioner::Jacobi<>::build()
  .on(exec))
  .with_criteria(
  gko::share(iter_stop),
  gko::share(res_stop))
  .on(exec);

  // make a shared pointer on the matrix A
  /* make_shared does not work : loses the pointer to the gko::executor ! */
  auto pA = std::shared_ptr<const gko::matrix::Csr<double, int>>(A.internal(), [](auto* p) { std::cout << " Did not delete the shared_pointer ! " << std::endl; });

  // Instantiate a ResidualLogger logger.
  auto res_logger = std::make_shared<ResidualLogger<double>>(exec, gko::lend(pA), gko::lend(b.internal()));

  if (output_level) {
    // Add the previously created logger to the solver factory.
    solver_factory->add_logger(res_logger);
  }

  // generate the solver
  auto cg_solver = solver_factory->generate(share(pA));

  // solve with timing
  auto tic = std::chrono::steady_clock::now();
  cg_solver->apply(lend(b.internal()), lend(x.internal()));
  auto toc = std::chrono::steady_clock::now();
  time += std::chrono::duration_cast<std::chrono::nanoseconds>(toc - tic);

  if (output_level) {
    // Print the table of the residuals obtained from the logger
    res_logger->write();
  }
}

void InternalLinearSolver::solve_BICGSTAB_jacobi(const Matrix& A, const Vector& b, Vector& x, auto& iter_stop, auto& res_stop, auto& exec, auto& conv_logger, const int& output_level, std::chrono::nanoseconds & time)
{
  std::cout << "----------- Solve with BICGSTAB + Jacobi : " << std::endl;

  auto solver_factory =
  gko::solver::Bicgstab<double>::build()
  .with_preconditioner(
  gko::preconditioner::Jacobi<>::build()
  .on(exec))
  .with_criteria(
  gko::share(iter_stop),
  gko::share(res_stop))
  .on(exec);

  // make a shared pointer on the matrix A
  /* make_shared does not work : loses the pointer to the gko::executor ! */
  auto pA = std::shared_ptr<const gko::matrix::Csr<double, int>>(A.internal(), [](auto* p) { std::cout << " Did not delete the shared_pointer ! " << std::endl; });

  // Instantiate a ResidualLogger logger.
  auto res_logger = std::make_shared<ResidualLogger<double>>(exec, gko::lend(pA), gko::lend(b.internal()));

  if (output_level) {
    // Add the previously created logger to the solver factory.
    solver_factory->add_logger(res_logger);
  }

  // generate the solver
  auto cg_solver = solver_factory->generate(share(pA));

  // solve with timing
  auto tic = std::chrono::steady_clock::now();
  cg_solver->apply(lend(b.internal()), lend(x.internal()));
  auto toc = std::chrono::steady_clock::now();
  time += std::chrono::duration_cast<std::chrono::nanoseconds>(toc - tic);

  if (output_level) {
    // Print the table of the residuals obtained from the logger
    res_logger->write();
  }
}

void InternalLinearSolver::solve_CG(const Matrix& A, const Vector& b, Vector& x, auto& iter_stop, auto& res_stop, auto& exec, auto& conv_logger, const int& output_level, std::chrono::nanoseconds & time)
{
  std::cout << "----------- Solve with CG + no PREC : " << std::endl;

  auto solver_factory =
  gko::solver::Cg<double>::build()
  .with_criteria(
  gko::share(iter_stop),
  gko::share(res_stop))
  .on(exec);

  // make a shared pointer on the matrix A
  /* make_shared does not work : loses the pointer to the gko::executor ! */
  auto pA = std::shared_ptr<const gko::matrix::Csr<double, int>>(A.internal(), [](auto* p) { std::cout << " Did not delete the shared_pointer ! " << std::endl; });

  // Instantiate a ResidualLogger logger.
  auto res_logger = std::make_shared<ResidualLogger<double>>(exec, gko::lend(pA), gko::lend(b.internal()));

  if (output_level) {
    // Add the previously created logger to the solver factory.
    solver_factory->add_logger(res_logger);
  }

  // generate the solver
  auto cg_solver = solver_factory->generate(share(pA));

  // solve with timing
  auto tic = std::chrono::steady_clock::now();
  cg_solver->apply(lend(b.internal()), lend(x.internal()));
  auto toc = std::chrono::steady_clock::now();
  time += std::chrono::duration_cast<std::chrono::nanoseconds>(toc - tic);

  if (output_level) {
    // Print the table of the residuals obtained from the logger
    res_logger->write();
  }
}

void InternalLinearSolver::solve_GMRES(const Matrix& A, const Vector& b, Vector& x, auto& iter_stop, auto& res_stop, auto& exec, auto& conv_logger, const int& output_level, std::chrono::nanoseconds & time)
{
  std::cout << "----------- Solve with GMRES + no PREC : " << std::endl;

  auto solver_factory =
  gko::solver::Gmres<double>::build()
  .with_criteria(
  gko::share(iter_stop),
  gko::share(res_stop))
  .on(exec);

  // make a shared pointer on the matrix A
  /* make_shared does not work : loses the pointer to the gko::executor ! */
  auto pA = std::shared_ptr<const gko::matrix::Csr<double, int>>(A.internal(), [](auto* p) { std::cout << " Did not delete the shared_pointer ! " << std::endl; });

  // Instantiate a ResidualLogger logger.
  auto res_logger = std::make_shared<ResidualLogger<double>>(exec, gko::lend(pA), gko::lend(b.internal()));

  if (output_level) {
    // Add the previously created logger to the solver factory.
    solver_factory->add_logger(res_logger);
  }

  // generate the solver
  auto cg_solver = solver_factory->generate(share(pA));

  // solve with timing
  auto tic = std::chrono::steady_clock::now();
  cg_solver->apply(lend(b.internal()), lend(x.internal()));
  auto toc = std::chrono::steady_clock::now();
  time += std::chrono::duration_cast<std::chrono::nanoseconds>(toc - tic);

  if (output_level) {
    // Print the table of the residuals obtained from the logger
    res_logger->write();
  }
}

void InternalLinearSolver::solve_BICG(const Matrix& A, const Vector& b, Vector& x, auto& iter_stop, auto& res_stop, auto& exec, auto& conv_logger, const int& output_level, std::chrono::nanoseconds & time)
{
  std::cout << "----------- Solve with BICG + no PREC : " << std::endl;

  auto solver_factory =
  gko::solver::Bicg<double>::build()
  .with_criteria(
  gko::share(iter_stop),
  gko::share(res_stop))
  .on(exec);

  // make a shared pointer on the matrix A
  /* make_shared does not work : loses the pointer to the gko::executor ! */
  auto pA = std::shared_ptr<const gko::matrix::Csr<double, int>>(A.internal(), [](auto* p) { std::cout << " Did not delete the shared_pointer ! " << std::endl; });

  // Instantiate a ResidualLogger logger.
  auto res_logger = std::make_shared<ResidualLogger<double>>(exec, gko::lend(pA), gko::lend(b.internal()));

  if (output_level) {
    // Add the previously created logger to the solver factory.
    solver_factory->add_logger(res_logger);
  }

  // generate the solver
  auto cg_solver = solver_factory->generate(share(pA));

  // solve with timing
  auto tic = std::chrono::steady_clock::now();
  cg_solver->apply(lend(b.internal()), lend(x.internal()));
  auto toc = std::chrono::steady_clock::now();
  time += std::chrono::duration_cast<std::chrono::nanoseconds>(toc - tic);

  if (output_level) {
    // Print the table of the residuals obtained from the logger
    res_logger->write();
  }
}

void InternalLinearSolver::solve_BICGSTAB(const Matrix& A, const Vector& b, Vector& x, auto& iter_stop, auto& res_stop, auto& exec, auto& conv_logger, const int& output_level, std::chrono::nanoseconds & time)
{
  std::cout << "----------- Solve with BICGSTAB + no PREC : " << std::endl;

  auto solver_factory =
  gko::solver::Bicgstab<double>::build()
  .with_criteria(
  gko::share(iter_stop),
  gko::share(res_stop))
  .on(exec);

  // make a shared pointer on the matrix A
  /* make_shared does not work : loses the pointer to the gko::executor ! */
  auto pA = std::shared_ptr<const gko::matrix::Csr<double, int>>(A.internal(), [](auto* p) { std::cout << " Did not delete the shared_pointer ! " << std::endl; });

  // Instantiate a ResidualLogger logger.
  auto res_logger = std::make_shared<ResidualLogger<double>>(exec, gko::lend(pA), gko::lend(b.internal()));

  if (output_level) {
    // Add the previously created logger to the solver factory.
    solver_factory->add_logger(res_logger);
  }

  // generate the solver
  auto cg_solver = solver_factory->generate(share(pA));

  // solve with timing
  auto tic = std::chrono::steady_clock::now();
  cg_solver->apply(lend(b.internal()), lend(x.internal()));
  auto toc = std::chrono::steady_clock::now();
  time += std::chrono::duration_cast<std::chrono::nanoseconds>(toc - tic);

  if (output_level) {
    // Print the table of the residuals obtained from the logger
    res_logger->write();
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
