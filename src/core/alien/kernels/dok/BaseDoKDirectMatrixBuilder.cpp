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
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "BaseDoKDirectMatrixBuilder.h"

#include <alien/core/impl/MultiMatrixImpl.h>
#include <alien/data/IMatrix.h>

#include "DoKBackEnd.h"
#include "DoKMatrixT.h"
#include "alien/handlers/scalar/MatrixBuilderFactory.h"

namespace Alien::Common
{

BaseDoKDirectMatrixBuilder::BaseDoKDirectMatrixBuilder(Alien::IMatrix& self, DirectMatrixOptions::ResetFlag reset_flag)
: m_matrix(self)
{
  m_matrix.impl()->lock();

  switch (reset_flag) {
  case DirectMatrixOptions::eResetProfile:
  case DirectMatrixOptions::eResetAllocation:
    m_matrix.impl()->release<BackEnd::tag::DoK>();
    break;
  default:
    // Nothing to do
    break;
  }

  m_impl = &m_matrix.impl()->get<BackEnd::tag::DoK>(true);

  switch (reset_flag) {
  case DirectMatrixOptions::eNoReset:
    break;
  case DirectMatrixOptions::eResetValues:
    m_impl->fill(0);
    break;
  default:
    // Nothing to do
    break;
  }
}

BaseDoKDirectMatrixBuilder::~BaseDoKDirectMatrixBuilder()
{
  assemble();
}

std::optional<Real> BaseDoKDirectMatrixBuilder::contribute(Arccore::Integer row, Arccore::Integer col, Arccore::Real value)
{
  return { m_impl->addNNZ(row, col, value) };
}

std::optional<Real> BaseDoKDirectMatrixBuilder::setNNZ(Arccore::Integer row, Arccore::Integer col, Arccore::Real value)
{
  m_impl->setNNZ(row, col, value);
  return { value };
}

bool BaseDoKDirectMatrixBuilder::assemble()
{
  m_impl->assemble();
  m_matrix.impl()->unlock();
  return true;
}

const ALIEN_EXPORT Alien::Common::MatrixBuilderFactory dok_builder_register(
AlgebraTraits<BackEnd::tag::DoK>::name(), [](IMatrix& matrix, DirectMatrixOptions::ResetFlag reset, [[maybe_unused]] DirectMatrixOptions::SymmetricFlag symmetry) { return std::make_unique<BaseDoKDirectMatrixBuilder>(matrix, reset); });

} // namespace Alien::Common
