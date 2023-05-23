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

#ifndef ALIEN_SIMPLECSRDIRECTBUILDER_H
#define ALIEN_SIMPLECSRDIRECTBUILDER_H

#include <map>

#include <alien/data/IMatrix.h>
#include <alien/data/utils/MatrixElement.h>

#define USE_VMAP

#ifdef USE_VMAP

#include <alien/utils/VMap.h>
#include <alien/handlers/scalar/IDirectMatrixBuilder.h>

#endif /* USE_VMAP */

namespace Arccore
{
class ITraceMng;
namespace MessagePassing
{
  class IMessagePassingMng;
}
} // namespace Arccore

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Alien
{

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

template <typename Scalar>
class SimpleCSRMatrix;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Common
{

  /*---------------------------------------------------------------------------*/
  /*---------------------------------------------------------------------------*/

  class ALIEN_EXPORT SimpleCSRDirectMatrixBuilder : public IDirectMatrixBuilder
  {
   public:
    using ResetFlag = DirectMatrixOptions::ResetFlag;
    using ReserveFlag = DirectMatrixOptions::ReserveFlag;
    using SymmetricFlag = DirectMatrixOptions::SymmetricFlag;

   public:
    using MatrixElement = MatrixElementT<SimpleCSRDirectMatrixBuilder>;

   public:
    SimpleCSRDirectMatrixBuilder(IMatrix& matrix, ResetFlag reset_flag,
                                 SymmetricFlag symmetric_flag = SymmetricFlag::eSymmetric);

    ~SimpleCSRDirectMatrixBuilder() override;

    SimpleCSRDirectMatrixBuilder(SimpleCSRDirectMatrixBuilder&) = delete;

    SimpleCSRDirectMatrixBuilder(SimpleCSRDirectMatrixBuilder&&) = delete;

    SimpleCSRDirectMatrixBuilder& operator=(const SimpleCSRDirectMatrixBuilder&) = delete;

    SimpleCSRDirectMatrixBuilder& operator=(SimpleCSRDirectMatrixBuilder&&) = delete;

    MatrixElement operator()(const Integer iIndex, const Integer jIndex)
    {
      return MatrixElement(iIndex, jIndex, *this);
    }

    void reserve(Arccore::Integer n, ReserveFlag flag = ReserveFlag::eResetReservation) override;

    void reserve(Arccore::ConstArrayView<Arccore::Integer> indices, Arccore::Integer n,
                 ReserveFlag flag = ReserveFlag::eResetReservation) override;

    void allocate() override;

    void addData(Arccore::Integer iIndex, Arccore::Integer jIndex, Arccore::Real value) override
    {
      if (!contribute(iIndex, jIndex, value).has_value()) {
#ifdef CHECKPROFILE_ON_FILLING
        hrow FatalErrorException(std::format("Cannot add ({}, {})", iIndex, jIndex);
#endif
      }
    }

    void addData(Arccore::Integer iIndex, Arccore::Real factor,
                 Arccore::ConstArrayView<Arccore::Integer> jIndexes,
                 Arccore::ConstArrayView<Arccore::Real> jValues) override;

    void setData(Arccore::Integer iIndex, Arccore::Integer jIndex, Arccore::Real value) override;

    void setData(Arccore::Integer iIndex, Arccore::Real factor,
                 Arccore::ConstArrayView<Arccore::Integer> jIndexes,
                 Arccore::ConstArrayView<Arccore::Real> jValues) override;

    void finalize() override;

    void squeeze() override;

    [[nodiscard]] Arccore::String stats() const override;

    [[nodiscard]] Arccore::String stats(Arccore::IntegerConstArrayView ids) const override;

    std::optional<double> contribute(Arccore::Integer iIndex, Arccore::Integer jIndex, Arccore::Real value) override;

   protected:
    IMatrix& m_matrix;

    SimpleCSRMatrix<Real>* m_matrix_impl;

    Integer m_local_offset = 0;
    Integer m_global_size = 0;
    Integer m_local_size = 0;
    Integer m_col_global_size = 0;
    ArrayView<Integer> m_row_starts;
    ArrayView<Integer> m_cols;
    ArrayView<Real> m_values;
    UniqueArray<Integer> m_row_sizes;

    ResetFlag m_reset_flag = DirectMatrixOptions::ResetFlag::eNoReset;
    bool m_allocated = false;
    bool m_finalized = false;
    bool m_symmetric_profile = false;

    Integer m_nproc = 0;
    IMessagePassingMng* m_parallel_mng;
    ITraceMng* m_trace;

#ifdef USE_VMAP
    typedef VMap<Integer, Real> ColValueData;
#else /* USE_VMAP */
    typedef std::map<Integer, Real> ColValueData;
#endif /* USE_VMAP */
    typedef std::map<Integer, ColValueData> ExtraRows;
    ExtraRows m_extras;

   private:
    void computeProfile(Arccore::ConstArrayView<Arccore::Integer> sizes);

    void updateProfile(Arccore::UniqueArray<Arccore::Integer>& row_starts,
                       Arccore::UniqueArray<Arccore::Integer>& cols,
                       Arccore::UniqueArray<Arccore::Real>& values);

    class IndexEnumerator;

    class FullEnumerator;

    template <typename Enumerator>
    void _stats(std::ostream& o, const Enumerator& e) const;

    void _startTimer() {}

    void _stopTimer() {}
  };

  /*---------------------------------------------------------------------------*/
  /*---------------------------------------------------------------------------*/

} // namespace Common

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // namespace Alien

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#endif //ALIEN_SIMPLECSRDIRECTBUILDER_H
