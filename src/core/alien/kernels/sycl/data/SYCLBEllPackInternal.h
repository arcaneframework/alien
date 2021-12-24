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

#include <alien/kernels/sycl/SYCLPrecomp.h>

#include <CL/sycl.hpp>

#include <alien/kernels/sycl/data/BEllPackStructInfo.h>

/*---------------------------------------------------------------------------*/

namespace Alien::SYCLInternal
{

  template<int BlockSize,typename IndexT>
  struct ALIEN_EXPORT StructInfoInternal
  {
    static const int                        block_size = BlockSize ;
    typedef IndexT                          index_type ;
    typedef cl::sycl::buffer<index_type, 1> index_buffer_type ;
    typedef cl::sycl::buffer<index_type, 1> IndexBufferType ;

    StructInfoInternal(std::size_t nrows,
                       std::size_t nnz,
                       std::size_t block_nrows,
                       std::size_t block_nnz,
                       int const* h_kcol,
                       int const* h_cols,
                       int const* h_block_row_offset) ;


    //IndexBufferType& getBlockRowOffset() { return m_block_row_offset ; }

    IndexBufferType& getBlockRowOffset() const { return  m_block_row_offset ; }

    IndexBufferType& getBlockCols() const { return m_block_cols; }

    IndexBufferType& getKCol() const { return m_kcol; }

    int const* kcol() const
    {
      return m_h_kcol.data() ;
    }

    int const* cols() const
    {
      return m_h_cols.data() ;
    }

    int const* dcol() const
    {
      getUpperDiagOffset() ;
      return m_h_dcol.data() ;
    }


    void getUpperDiagOffset() const ;
    void computeLowerUpperMask() const ;

    IndexBufferType& getLowerMask() const;
    IndexBufferType& getUpperMask() const;

    std::size_t m_nrows       = 0 ;
    std::size_t m_nnz         = 0 ;
    std::size_t m_block_nrows = 0 ;
    std::size_t m_block_nnz   = 0 ;

    std::vector<index_type> m_h_kcol ;
    std::vector<index_type> m_h_cols ;
    std::vector<index_type> m_h_block_cols ;

    mutable std::vector<index_type>  m_h_dcol ;
    mutable IndexBufferType          m_block_row_offset ;
    mutable IndexBufferType          m_block_cols ;
    mutable IndexBufferType          m_kcol ;

    mutable std::unique_ptr<IndexBufferType> m_lower_mask ;
    mutable std::unique_ptr<IndexBufferType> m_upper_mask ;
  };

/*---------------------------------------------------------------------------*/

template <typename ValueT, int BlockSize>
class MatrixInternal
{
 public:
  typedef ValueT   ValueType;
  typedef ValueT   value_type;
  static const int block_size = BlockSize ;

  typedef MatrixInternal<ValueType,BlockSize>   ThisType;
  typedef BEllPackStructInfo<BlockSize,int>     ProfileType;
  typedef typename ProfileType::InternalType    InternalProfileType ;
  typedef typename
      InternalProfileType::IndexBufferType      IndexBufferType ;

  typedef cl::sycl::buffer<value_type, 1>       value_buffer_type ;

  typedef cl::sycl::buffer<value_type, 1>       ValueBufferType ;

  typedef cl::sycl::queue                       QueueType ;

 public:
  MatrixInternal(ProfileType const* profile) ;

  ~MatrixInternal() {}

  bool setMatrixValues(ValueType const* values, bool only_host) ;
  bool setMatrixValuesFromHost() ;

  bool needUpdate() ;
  void notifyChanges() ;
  void endUpdate() ;


  void mult(ValueBufferType& x, ValueBufferType& y) const;

  void mult(ValueBufferType& x, ValueBufferType& y, QueueType& queue) const;

  void addLMult(ValueType alpha, ValueBufferType& x, ValueBufferType& y) const;
  void addUMult(ValueType alpha, ValueBufferType& x, ValueBufferType& y) const;

  void addLMult(ValueType alpha, ValueBufferType& x, ValueBufferType& y, QueueType& queue) const;
  void addUMult(ValueType alpha, ValueBufferType& x, ValueBufferType& y, QueueType& queue) const;

  void multInvDiag(ValueBufferType& y) const;

  void multInvDiag(ValueBufferType& y, QueueType& queue) const;

  void computeInvDiag(ValueBufferType& y) const;

  void computeInvDiag(ValueBufferType& y, QueueType& queue) const;

  ValueBufferType& getValues() { return m_values; }

  ValueBufferType const getValues() const { return m_values; }

  ProfileType* getProfile() { return m_profile; }

  ProfileType const* getProfile() const { return m_profile; }

  ValueType const* getHCsrData() const {
    return m_h_csr_values.data() ;
  }

  ValueType * getHCsrData() {
    return m_h_csr_values.data() ;
  }



  ProfileType const* m_profile = nullptr;

  std::vector<ValueType>  m_h_csr_values ;
  std::vector<ValueType>  m_h_values ;
  mutable ValueBufferType m_values ;
  bool                    m_values_is_update = false ;

};

/*---------------------------------------------------------------------------*/

} // namespace Alien::SYCLInternal

/*---------------------------------------------------------------------------*/
