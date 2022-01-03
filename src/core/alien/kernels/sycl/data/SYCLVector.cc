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

#include <cassert>

#include "SYCLVectorInternal.h"
#include "SYCLVector.h"
/* Author : desrozis at Tue Feb 19 10:40:56 2013
 * Generated by createNew
 */

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

using namespace Arccore;

namespace Alien
{

template <typename ValueT>
SYCLVector<ValueT>::~SYCLVector()
{
  delete m_internal;
}

template <typename ValueT>
void SYCLVector<ValueT>::allocate()
{
  delete m_internal;
  m_h_values.resize(m_local_size);
  m_internal = new VectorInternal(m_h_values.data(), m_local_size);
}

template <typename ValueT>
void SYCLVector<ValueT>::resize(Integer alloc_size) const
{
  delete m_internal;
  m_h_values.resize(alloc_size);
  m_internal = new VectorInternal(m_h_values.data(), alloc_size);
}

template <typename ValueT>
void SYCLVector<ValueT>::clear()
{
  delete m_internal;
  m_internal = nullptr;
  std::vector<ValueType>().swap(m_h_values);
}

template <typename ValueT>
void SYCLVector<ValueT>::setValuesFromHost()
{
  delete m_internal;
  m_internal = new VectorInternal(m_h_values.data(), m_local_size);
}

template <typename ValueT>
void SYCLVector<ValueT>::setValues(std::size_t size, ValueType const* ptr)
{
  delete m_internal ;
  m_h_values.resize(m_local_size);
  std::copy(ptr, ptr + size, m_h_values.begin());
  m_internal = new VectorInternal(m_h_values.data(), m_local_size);
}

template <typename ValueT>
void SYCLVector<ValueT>::copyValuesTo(std::size_t size, ValueType* ptr) const
{
  if (m_internal)
    m_internal->copyValuesToHost(size, ptr);
}

/*---------------------------------------------------------------------------*/

template class ALIEN_EXPORT SYCLVector<Real>;

/*---------------------------------------------------------------------------*/

} // namespace Alien

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
