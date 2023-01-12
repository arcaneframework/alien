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

#include <alien/handlers/scalar/BaseDirectMatrixBuilder.h>
#include <alien/utils/MoveObject.h>

#include <alien/move/data/MatrixData.h>

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Alien::Move
{

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

class DirectMatrixBuilder : protected MoveObject<MatrixData>
{
 public:
  using MatrixElement = MatrixElementT<DirectMatrixBuilder>;

 public:
  DirectMatrixBuilder(MatrixData&& matrix, const DirectMatrixOptions::ResetFlag reset_flag,
                      const DirectMatrixOptions::SymmetricFlag symmetric_flag = DirectMatrixOptions::SymmetricFlag::eSymmetric)
  : MoveObject<MatrixData>(std::move(matrix))
  , m_data(reference(), reset_flag, symmetric_flag)
  {}

  virtual ~DirectMatrixBuilder() = default;

  MatrixData&& release()
  {
    m_data.finalize();

    return MoveObject<MatrixData>::release();
  }

  void reserve(Arccore::Integer n, DirectMatrixOptions::ReserveFlag flag = DirectMatrixOptions::ReserveFlag::eResetReservation)
  {
    m_data.reserve(n, flag);
  }

  void reserve(Arccore::ConstArrayView<Arccore::Integer> indices, Arccore::Integer n,
               DirectMatrixOptions::ReserveFlag flag = DirectMatrixOptions::ReserveFlag::eResetReservation)
  {
    m_data.reserve(indices, n, flag);
  }

  void allocate()
  {
    m_data.allocate();
  }

  MatrixElement operator()(const Integer iIndex, const Integer jIndex)
  {
    return MatrixElement(iIndex, jIndex, *this);
  }

  void addData(Arccore::Integer iIndex, Arccore::Integer jIndex, Arccore::Real value)
  {
    m_data.addData(iIndex, jIndex, value);
  }

  void addData(Arccore::Integer iIndex, Arccore::Real factor,
               Arccore::ConstArrayView<Arccore::Integer> jIndexes,
               Arccore::ConstArrayView<Arccore::Real> jValues)
  {
    m_data.addData(iIndex, factor, jIndexes, jValues);
  }

  void setData(Arccore::Integer iIndex, Arccore::Integer jIndex, Arccore::Real value)
  {
    m_data.setData(iIndex, jIndex, value);
  }

  void setData(Arccore::Integer iIndex, Arccore::Real factor,
               Arccore::ConstArrayView<Arccore::Integer> jIndexes,
               Arccore::ConstArrayView<Arccore::Real> jValues)
  {
    m_data.setData(iIndex, factor, jIndexes, jValues);
  }

  void finalize()
  {
    m_data.finalize();
  }

  void squeeze()
  {
    m_data.squeeze();
  }

  [[nodiscard]] Arccore::String stats() const
  {
    return m_data.stats();
  }
  [[nodiscard]] Arccore::String stats(Arccore::IntegerConstArrayView ids) const
  {
    return m_data.stats(ids);
  }

 private:
  Common::DirectMatrixBuilder m_data;
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // namespace Alien::Move

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
