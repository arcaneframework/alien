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

#include <arccore/message_passing_mpi/MpiMessagePassingMng.h>

#include <alien/core/backend/LinearSolver.h>
#include <alien/expression/solver/SolverStater.h>

#include <alien/core/impl/IMatrixImpl.h>
#include <alien/core/impl/IVectorImpl.h>

#include <alien/hypre/backend.h>
#include <alien/hypre/export.h>
#include <alien/hypre/options.h>

#include "hypre_matrix.h"
#include "hypre_vector.h"

namespace Alien
{
class PluginLinearSolver : public IInternalLinearSolver
, public ObjectWithTrace
{
 public:
  typedef SolverStatus Status;

  PluginLinearSolver() = default;

  explicit PluginLinearSolver(const BackEnd::Options& options)
  : m_status()
  , m_init_time(0.0)
  , m_total_solve_time(0.0)
  , m_solve_num(0)
  , m_total_iter_num(0)
  , m_stat()
  , m_options(options)
  {}

  virtual ~PluginLinearSolver() = default;

 public:
  Arccore::String getBackEndName() const;

  // Nothing to do
  void updateParallelMng(ALIEN_UNUSED_PARAM Arccore::MessagePassing::IMessagePassingMng* pm) {}

  bool solve(const IMatrixImpl& A, const IVectorImpl& b, IVectorImpl& x);

  bool hasParallelSupport() const { return true; }

  //! Etat du solveur
  const Status& getStatus() const;

  const SolverStat& getSolverStat() const { return m_stat; }

  const BackEndId backEndName() const { return "hypre"; }

 private:
  bool _solve(const Matrix& A, const Vector& b, Vector& x);

 private:
  Status m_status;

  Arccore::Real m_init_time;
  Arccore::Real m_total_solve_time;
  Arccore::Integer m_solve_num;
  Arccore::Integer m_total_iter_num;

  SolverStat m_stat;
  BackEnd::Options m_options;

 private:
  void checkError(const Arccore::String& msg, int ierr, int skipError = 0) const;
};
} // namespace Alien
