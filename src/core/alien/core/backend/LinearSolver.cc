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

/*!
 * \file LinearSolverT.h
 * \brief LinearSolverT.h
 */

#include <alien/core/backend/LinearSolver.h>

#include <alien/expression/solver/SolverStat.h>

#include <alien/core/impl/MultiMatrixImpl.h>
#include <alien/core/impl/MultiVectorImpl.h>
#include <alien/data/IMatrix.h>
#include <alien/data/IVector.h>

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Alien
{

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

LinearSolver::LinearSolver(std::string soFile)
{
  // FIXME: add error checking

  m_handle = dlopen(soFile.c_str(), RTLD_NOW | RTLD_LOCAL);
  m_plugin_create = (BackEnd::IPlugin*(*)()) dlsym(m_handle, "create");
  m_plugin_destroy = (void (*)(BackEnd::IPlugin*)) dlsym(m_handle, "destroy");

  m_plugin.reset(m_plugin_create());
  m_solver = m_plugin->solver_factory();

  m_backEndId = m_solver->backEndName();

  m_plugin->registerMatrixConverters(MultiMatrixImpl::m_matrixConverters);
  m_plugin->registerVectorConverters(MultiVectorImpl::m_vectorConverters);
  m_plugin->registerMatrixFactory(MultiMatrixImpl::m_matrixFactory);
  m_plugin->registerVectorFactory(MultiVectorImpl::m_vectorFactory);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

LinearSolver::~LinearSolver()
{
  // FIXME: m_handle + plugin.destroy()?
  m_plugin_destroy(m_plugin.release());
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

String
LinearSolver::getBackEndName() const
{
  return m_solver->backEndName();
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

/*!
 * \todo Check the comment below and fix if necessary
 */
bool LinearSolver::solve(const IMatrix& A, const IVector& b, IVector& x)
{
  auto dist = A.impl()->distribution();
  // solve is a global call
  // when redistribution, matrix or dist could be null on some subsets of procs
  // so we return true to don't block
  // FIXME: sync result ??
  if (A.impl() == nullptr || dist.parallelMng() == nullptr)
    return true;
  // solver is not parallel but dist is
  if (not m_solver->hasParallelSupport() && dist.isParallel()) {
    throw FatalErrorException("solver is not parallel");
  }
  else {
    m_solver->updateParallelMng(A.impl()->distribution().parallelMng());
  }
  // m_solver->getSolverStat().startPrepareMeasure();
  // FIXME
  const auto& matrix = A.impl()->get(m_backEndId);
  const auto& rhs = b.impl()->get(m_backEndId);
  auto& sol = x.impl()->get(m_backEndId, true);
  // m_solver->getSolverStat().stopPrepareMeasure();
  return m_solver->solve(matrix, rhs, sol);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

void LinearSolver::init()
{
  m_solver->init();
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

void LinearSolver::end()
{
  m_solver->end();
}

void LinearSolver::updateParallelMng(Arccore::MessagePassing::IMessagePassingMng* pm)
{
  m_solver->updateParallelMng(pm);
}
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

const SolverStat&
LinearSolver::getSolverStat() const
{
  return m_solver->getSolverStat();
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

bool LinearSolver::hasParallelSupport() const
{
  return m_solver->hasParallelSupport();
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

const SolverStatus&
LinearSolver::getStatus() const
{
  return m_solver->getStatus();
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

IInternalLinearSolver*
LinearSolver::implem()
{
  return m_solver.get();
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

std::shared_ptr<ILinearAlgebra>
LinearSolver::algebra() const
{
  return m_solver->algebra();
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // namespace Alien

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
