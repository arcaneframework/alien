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

// -*- C++ -*-
#ifndef ALIEN_COMMON_BUILDER_SCALAR_DIRECTMATRIXBUILDER_H
#define ALIEN_COMMON_BUILDER_SCALAR_DIRECTMATRIXBUILDER_H

#include <arccore/base/BaseTypes.h>

#include <alien/data/IMatrix.h>

namespace Alien
{
struct ALIEN_EXPORT DirectMatrixOptions
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

namespace Common
{

  /*---------------------------------------------------------------------------*/
  /*---------------------------------------------------------------------------*/

  class ALIEN_EXPORT IDirectMatrixBuilder
  {
   public:
    using ReserveFlag = DirectMatrixOptions::ReserveFlag;

   public:
    virtual ~IDirectMatrixBuilder() = default;

   public:
    virtual void reserve(Arccore::Integer n, ReserveFlag flag) = 0;

    virtual void reserve(Arccore::ConstArrayView<Arccore::Integer> indices, Arccore::Integer n,
                         ReserveFlag flag) = 0;

    virtual void allocate() = 0;

    virtual void addData(Arccore::Integer iIndex, Arccore::Integer jIndex, Arccore::Real value) = 0;

    virtual void addData(Arccore::Integer iIndex, Arccore::Real factor,
                         Arccore::ConstArrayView<Arccore::Integer> jIndexes,
                         Arccore::ConstArrayView<Arccore::Real> jValues) = 0;

    virtual void setData(Arccore::Integer iIndex, Arccore::Integer jIndex, Arccore::Real value) = 0;

    virtual void setData(Arccore::Integer iIndex, Arccore::Real factor,
                         Arccore::ConstArrayView<Arccore::Integer> jIndexes,
                         Arccore::ConstArrayView<Arccore::Real> jValues) = 0;

    virtual void finalize() = 0;

    virtual void squeeze() = 0;

    [[nodiscard]] virtual Arccore::String stats() const = 0;

    [[nodiscard]] virtual Arccore::String stats(Arccore::IntegerConstArrayView ids) const = 0;

    virtual std::optional<Arccore::Real> contribute(Arccore::Integer row, Arccore::Integer col, Arccore::Real value) = 0;
  };

  /*---------------------------------------------------------------------------*/
  /*---------------------------------------------------------------------------*/

  extern ALIEN_EXPORT std::unique_ptr<Alien::Common::IDirectMatrixBuilder> directMatrixBuilderFactory(IMatrix& matrix, DirectMatrixOptions::ResetFlag reset_flag,
                                                                                                      DirectMatrixOptions::SymmetricFlag symmetric_flag);

} // namespace Common

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // namespace Alien

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif /* ALIEN_COMMON_BUILDER_SCALAR_DIRECTMATRIXBUILDER_H */
