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

/*#include <HYPRE_parcsr_ls.h>
#include <HYPRE_parcsr_mv.h>*/

namespace Alien
{
// Compile TrilinosLinearSolver.
template class ALIEN_TRILINOS_EXPORT LinearSolver<BackEnd::tag::trilinos>;

} // namespace Alien

namespace Alien::Trilinos
{
void InternalLinearSolver::checkError(
const Arccore::String& msg, int ierr, int skipError) const
{
/*  if (ierr != 0 and (ierr & ~skipError) != 0) {
    char hypre_error_msg[256];
    HYPRE_DescribeError(ierr, hypre_error_msg);
    alien_fatal([&] {
      cout() << msg << " failed : " << hypre_error_msg << "[code=" << ierr << "]";
    });
  }*/
}

bool InternalLinearSolver::solve(const Matrix& A, const Vector& b, Vector& x)
{
  std::cout << "call to solver : " ;
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
