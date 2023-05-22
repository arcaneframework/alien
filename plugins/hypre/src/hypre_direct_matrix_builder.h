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

#ifndef ALIEN_HYPREDIRECTMATRIXBUILDER_H
#define ALIEN_HYPREDIRECTMATRIXBUILDER_H

#include <alien/data/IMatrix.h>
#include <alien/data/utils/MatrixElement.h>

#include <arccore/trace/ITraceMng.h>
#include <arccore/message_passing/IMessagePassingMng.h>

#include "hypre_matrix.h"

namespace Alien
{
struct DirectMatrixOptions
{
  enum ResetFlag
  {
    eNoReset,
    eResetValues,
    eResetProfile,
    eResetAllocation
  };
  enum ReserveFlag
  {
    eResetReservation,
    eExtendReservation
  };
  enum SymmetricFlag
  {
    eSymmetric,
    eUnSymmetric
  };
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Hypre
{

  /*---------------------------------------------------------------------------*/
  /*---------------------------------------------------------------------------*/

  class ALIEN_EXPORT HypreDirectMatrixBuilder
  {
   public:
    using ResetFlag = DirectMatrixOptions::ResetFlag;
    using ReserveFlag = DirectMatrixOptions::ReserveFlag;
    using SymmetricFlag = DirectMatrixOptions::SymmetricFlag;

    using MatrixElement = MatrixElementT<HypreDirectMatrixBuilder>;

    HypreDirectMatrixBuilder(IMatrix& matrix, ResetFlag reset_flag,
                             SymmetricFlag symmetric_flag = SymmetricFlag::eSymmetric);

    virtual ~HypreDirectMatrixBuilder()
    {
      finalize();
    }

    HypreDirectMatrixBuilder(HypreDirectMatrixBuilder&) = delete;

    HypreDirectMatrixBuilder(HypreDirectMatrixBuilder&&) = delete;

    HypreDirectMatrixBuilder& operator=(const HypreDirectMatrixBuilder&) = delete;

    HypreDirectMatrixBuilder& operator=(HypreDirectMatrixBuilder&&) = delete;

    MatrixElement operator()(const Integer iIndex, const Integer jIndex)
    {
      return { iIndex, jIndex, *this };
    }

    void reserve(Arccore::Integer n, ReserveFlag flag = ReserveFlag::eResetReservation)
    {
      // Nothing yet
    }

    void reserve(Arccore::ConstArrayView<Arccore::Integer> indices, Arccore::Integer n,
                 ReserveFlag flag = ReserveFlag::eResetReservation)
    {
      // Nothing yet
    }

    void allocate()
    {
      // Nothing yet
    }

    void addData(Arccore::Integer iIndex, Arccore::Integer jIndex, Arccore::Real value);

    void addData(Arccore::Integer iIndex, Arccore::Real factor,
                 Arccore::ConstArrayView<Arccore::Integer> jIndexes,
                 Arccore::ConstArrayView<Arccore::Real> jValues);

    void setData(Arccore::Integer iIndex, Arccore::Integer jIndex, Arccore::Real value);

    void setData(Arccore::Integer iIndex, Arccore::Real factor,
                 Arccore::ConstArrayView<Arccore::Integer> jIndexes,
                 Arccore::ConstArrayView<Arccore::Real> jValues);

    void finalize();

    void squeeze()
    {
      // do nothing
    }

    [[nodiscard]] Arccore::String stats() const
    {
      // Do nothing
      return { "" };
    }

    [[nodiscard]] Arccore::String stats(Arccore::IntegerConstArrayView ids) const
    {
      // Do nothing
      return { "" };
    }

   private:
    IMatrix& m_matrix;

    Matrix* m_matrix_impl;
  };

  /*---------------------------------------------------------------------------*/
  /*---------------------------------------------------------------------------*/

} // namespace Hypre

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // namespace Alien

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif //ALIEN_HYPREDIRECTMATRIXBUILDER_H
