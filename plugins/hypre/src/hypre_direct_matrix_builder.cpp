/*
 * Copyright 2023 IFPEN-CEA
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
 *  SPDX-License-Identifier: Apache-2.0
 */

//
// Created by chevalierc on 17/05/23.
//

#include "hypre_direct_matrix_builder.h"
#include "alien/hypre/export.h"

#include <alien/core/impl/MultiMatrixImpl.h>
#include <alien/handlers/scalar/MatrixBuilderFactory.h>

#include <alien/hypre/backend.h>

namespace Alien::Hypre
{
HypreDirectMatrixBuilder::HypreDirectMatrixBuilder(Alien::IMatrix& matrix,
                                                   Hypre::HypreDirectMatrixBuilder::ResetFlag reset_flag,
                                                   Hypre::HypreDirectMatrixBuilder::SymmetricFlag symmetric_flag)
: m_matrix(matrix)
{

  m_matrix.impl()->lock();
  m_matrix_impl = &m_matrix.impl()->get<BackEnd::tag::hypre>(true);
}

void HypreDirectMatrixBuilder::finalize()
{
  m_matrix_impl->assemble();
  m_matrix.impl()->unlock();
}

void HypreDirectMatrixBuilder::setData(Arccore::Integer iIndex, Arccore::Integer jIndex, Arccore::Real value)
{
  m_matrix_impl->setRowValues(iIndex, ConstArrayView<Integer>(1, &jIndex), ConstArrayView<Real>(1, &value));
}

void HypreDirectMatrixBuilder::setData(Arccore::Integer iIndex, Arccore::Real factor,
                                       Arccore::ConstArrayView<Arccore::Integer> jIndexes,
                                       Arccore::ConstArrayView<Arccore::Real> jValues)
{
  if (factor != 1.0) {
    throw FatalErrorException("Scaling is not supported");
  }

  m_matrix_impl->setRowValues(iIndex, jIndexes, jValues);
}

void HypreDirectMatrixBuilder::addData(Arccore::Integer iIndex, Arccore::Integer jIndex, Arccore::Real value)
{
  m_matrix_impl->addRowValues(iIndex, ConstArrayView<Integer>(1, &jIndex), ConstArrayView<Real>(1, &value));
}

void HypreDirectMatrixBuilder::addData(Arccore::Integer iIndex, Arccore::Real factor,
                                       Arccore::ConstArrayView<Arccore::Integer> jIndexes,
                                       Arccore::ConstArrayView<Arccore::Real> jValues)
{
  if (factor != 1.0) {
    throw FatalErrorException("Scaling is not supported");
  }

  m_matrix_impl->addRowValues(iIndex, jIndexes, jValues);
}

const ALIEN_HYPRE_EXPORT Alien::Common::MatrixBuilderFactory hypre_builder_register(
AlgebraTraits<BackEnd::tag::hypre>::name(), [](IMatrix& matrix, DirectMatrixOptions::ResetFlag reset, DirectMatrixOptions::SymmetricFlag symmetry) { return std::make_unique<HypreDirectMatrixBuilder>(matrix, reset, symmetry); });

}; // namespace Alien::Hypre
