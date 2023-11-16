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

#pragma once

#include <alien/handlers/scalar/IDirectMatrixBuilder.h>
#include <alien/handlers/scalar/MatrixBuilderFactory.h>
#include <alien/utils/MoveObject.h>

#include <alien/move/data/MatrixData.h>
#include <alien/data/utils/MatrixElement.h>

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Alien::Move
{

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

class ALIEN_MOVESEMANTIC_EXPORT DirectMatrixBuilder
{
 public:
  using ResetFlag = DirectMatrixOptions::ResetFlag;
  using ReserveFlag = DirectMatrixOptions::ReserveFlag;
  using SymmetricFlag = DirectMatrixOptions::SymmetricFlag;

  using MatrixElement = MatrixElementT<DirectMatrixBuilder>;

  DirectMatrixBuilder(MatrixData&& matrix, const ResetFlag reset_flag,
                      const SymmetricFlag symmetric_flag = SymmetricFlag::eSymmetric, std::optional<BackEndId> backend = std::nullopt)
  : m_data(std::move(matrix))
  {
    auto factory = Common::MatrixBuilderFactory::getDefault();
    if (backend.has_value()) {
      factory = Common::MatrixBuilderFactory::get(backend.value()).value_or(factory);
    }
    m_builder = factory(m_data, reset_flag, symmetric_flag);
  }

  MatrixElement operator()(const Integer iIndex, const Integer jIndex)
  {
    return MatrixElement(iIndex, jIndex, *this);
  }

  void reserve(Arccore::Integer n,
               ReserveFlag flag = ReserveFlag::eResetReservation)
  {
    m_builder->reserve(n, flag);
  }

  void reserve(Arccore::ConstArrayView<Arccore::Integer> indices, Arccore::Integer n,
               ReserveFlag flag = ReserveFlag::eResetReservation)
  {
    m_builder->reserve(indices, n, flag);
  }

  void allocate()
  {
    m_builder->allocate();
  }

  void addData(Arccore::Integer iIndex, Arccore::Integer jIndex, Arccore::Real value)
  {
    m_builder->addData(iIndex, jIndex, value);
  }

  void addData(Arccore::Integer iIndex, Arccore::Real factor,
               Arccore::ConstArrayView<Arccore::Integer> jIndexes,
               Arccore::ConstArrayView<Arccore::Real> jValues)
  {
    m_builder->addData(iIndex, factor, jIndexes, jValues);
  }

  void setData(Arccore::Integer iIndex, Arccore::Integer jIndex, Arccore::Real value)
  {
    m_builder->setData(iIndex, jIndex, value);
  }

  void setData(Arccore::Integer iIndex, Arccore::Real factor,
               Arccore::ConstArrayView<Arccore::Integer> jIndexes,
               Arccore::ConstArrayView<Arccore::Real> jValues)
  {
    m_builder->setData(iIndex, factor, jIndexes, jValues);
  }

  void finalize()
  {
    m_builder->finalize();
  }

  void squeeze()
  {
    m_builder->squeeze();
  }

  [[nodiscard]] Arccore::String stats() const
  {
    return m_builder->stats();
  }

  [[nodiscard]] Arccore::String stats(Arccore::IntegerConstArrayView ids) const
  {
    return m_builder->stats(ids);
  }

  MatrixData&& release()
  {
    m_builder->finalize();
    m_builder.reset(nullptr);

    return std::move(m_data);
  }

  std::optional<Arccore::Real> contribute(Arccore::Integer row, Arccore::Integer col, Arccore::Real value)
  {
    if (!m_builder) {
      return std::nullopt;
    }

    return m_builder->contribute(row, col, value);
  }

 private:
  MatrixData m_data;
  std::unique_ptr<Alien::Common::IDirectMatrixBuilder> m_builder;
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // namespace Alien::Move

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
