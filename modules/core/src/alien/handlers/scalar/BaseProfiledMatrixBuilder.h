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
#ifndef ALIEN_COMMON_SCALAR_PROFILEDMATRIXBUILDER_H
#define ALIEN_COMMON_SCALAR_PROFILEDMATRIXBUILDER_H
/* Author : havep at Tue Aug 28 13:56:24 2012
 * Generated by createNew
 */

#include <alien/data/IMatrix.h>
#include <alien/data/utils/MatrixElement.h>

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

struct ProfiledMatrixOptions
{
  enum ResetFlag
  {
    eKeepValues,
    eResetValues
  };
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Common
{

  /*---------------------------------------------------------------------------*/
  /*---------------------------------------------------------------------------*/

  class ALIEN_EXPORT ProfiledMatrixBuilder
  {
   public:
    using ResetFlag = ProfiledMatrixOptions::ResetFlag;

   public:
    using MatrixElement = MatrixElementT<ProfiledMatrixBuilder>;

   public:
    ProfiledMatrixBuilder(IMatrix& matrix, ResetFlag reset_values);

    virtual ~ProfiledMatrixBuilder();

    ProfiledMatrixBuilder(const ProfiledMatrixBuilder&) = delete;
    ProfiledMatrixBuilder(ProfiledMatrixBuilder&&) = delete;
    ProfiledMatrixBuilder& operator=(const ProfiledMatrixBuilder&) = delete;
    ProfiledMatrixBuilder& operator=(ProfiledMatrixBuilder&&) = delete;

   public:
    inline MatrixElement operator()(const Integer iIndex, const Integer jIndex)
    {
      return MatrixElement(iIndex, jIndex, *this);
    }

    Arccore::Real getData(Arccore::Integer iIndex, Arccore::Integer jIndex) const;

    void addData(Arccore::Integer iIndex, Arccore::Integer jIndex, Arccore::Real value);

    void addData(Arccore::Integer iIndex, Arccore::Real factor,
                 Arccore::ConstArrayView<Arccore::Integer> jIndexes,
                 Arccore::ConstArrayView<Arccore::Real> jValues);

    void setData(Arccore::Integer iIndex, Arccore::Integer jIndex, Arccore::Real value);

    void setData(Arccore::Integer iIndex, Arccore::Real factor,
                 Arccore::ConstArrayView<Arccore::Integer> jIndexes,
                 Arccore::ConstArrayView<Arccore::Real> jValues);

    void finalize();

   private:
    bool isLocal(Integer jIndex) const
    {
      return (jIndex >= m_local_offset) && (jIndex < m_next_offset);
    }

    void _startTimer() {}
    void _stopTimer() {}

   private:
    IMatrix& m_matrix;
    SimpleCSRMatrix<Real>* m_matrix_impl;

    Integer m_local_offset;
    Integer m_local_size;
    Integer m_next_offset;
    ConstArrayView<Integer> m_row_starts;
    ConstArrayView<Integer> m_cols;
    ConstArrayView<Integer> m_local_row_size;
    ArrayView<Real> m_values;
    bool m_finalized;
  };

  /*---------------------------------------------------------------------------*/
  /*---------------------------------------------------------------------------*/

} // namespace Common

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // namespace Alien

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif /* ALIEN_COMMON_SCALAR_PROFILEDMATRIXBUILDER_H */
