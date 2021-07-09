﻿// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2020 IFPEN-CEA
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* ArrayRange.h                                                (C) 2000-2019 */
/*                                                                           */
/* Intervalle sur les Array, ArrayView, ConstArrayView, ...                  */
/*---------------------------------------------------------------------------*/
#ifndef ARCCORE_BASE_ARRAYRANGE_H
#define ARCCORE_BASE_ARRAYRANGE_H
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include "arccore/base/ArrayIterator.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Arccore
{

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*!
 * \brief Intervalle sur les classes tableau de %Arccore.
 *
 * Cette classe est utilisée pour adapter les classes tableaux aux
 * itérateurs de la STL. Elle fourniy les méthodes telles que begin()/end().
 */
template<typename T>
class ArrayRange
{
 public:

  typedef std::iterator_traits<T> _TraitsType;

 public:

  typedef typename _TraitsType::value_type value_type;
  typedef typename _TraitsType::difference_type difference_type;
  typedef typename _TraitsType::reference reference;
  typedef typename _TraitsType::pointer pointer;
  typedef const value_type* const_pointer;
  //! Type de l'itérateur sur un élément du tableau
  typedef ArrayIterator<pointer> iterator;
  //! Type de l'itérateur constant sur un élément du tableau
  typedef ArrayIterator<const_pointer> const_iterator;

 public:

  //! Construit un interval vide.
  ArrayRange() ARCCORE_NOEXCEPT : m_begin(nullptr), m_end(nullptr) { }
  //! Construit un interval allant de \a abegin à \a aend.
  ArrayRange(pointer abegin,pointer aend) ARCCORE_NOEXCEPT : m_begin(abegin), m_end(aend) { }

 public:

  //! Retourne un iterateur sur le premier élément du tableau
  iterator begin() { return iterator(m_begin); }
  //! Retourne un iterateur sur le premier élément après la fin du tableau
  iterator end() { return iterator(m_end); }
  //! Retourne un iterateur constant sur le premier élément du tableau
  const_iterator begin() const { return const_iterator(m_begin); }
  //! Retourne un iterateur constant sur le premier élément après la fin du tableau
  const_iterator end() const { return const_iterator(m_end); }

  //! Pointeur sur le tableau sous-jacent.
  value_type* data() { return m_begin; }
  //! Pointeur constant sur le tableau sous-jacent.
  const value_type* data() const { return m_begin; }
  //! Indique si le tableau est vide.
  bool empty() const { return m_end==m_begin; }

 private:

  T m_begin;
  T m_end;
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // End namespace Arccore

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif  
