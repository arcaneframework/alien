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
/* Iterator.h                                                  (C) 2000-2018 */
/*                                                                           */
/* Iterateurs (obsolète).                                                    */
/*---------------------------------------------------------------------------*/
#ifndef ARCCORE_BASE_ITERATOR_H
#define ARCCORE_BASE_ITERATOR_H
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include "arccore/base/ArccoreGlobal.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Arccore
{

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*!
 * \internal
 * \brief Interval d'itération.
 * \ingroup Collection
 Cette classe gère un interval d'itération avec un début et une fin. Il
 permet de construire simplement un couple d'itérateur pour itérer
 sur tout le container.
*/
template<class IT,class R,class P,class V>
class IteratorBase
{
 public:

  IteratorBase(IT b,IT e) : m_begin(b), m_end(e) {}

  void operator++(){ ++m_begin; }
  void operator--(){ --m_begin; }
  R operator* () const { return *m_begin; }
  V operator->() const { return &(*m_begin); }
  bool notEnd() const { return m_begin!=m_end; }
  bool operator()() const { return notEnd(); }
  IT current() const { return m_begin; }
  IT end() const { return m_end; }
	
 private:

  IT m_begin; //!< Itérateur sur l'élément courant
  IT m_end;   //!< Itérateur sur la fin du container.
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*!
 * \internal
 * \brief interval d'itérateurs
 * \ingroup Collection
 Cette classe gère un couple d'itérateurs autorisant la modification des
 éléments du container.
 */
template<class T>
class IterT
: public IteratorBase<typename T::iterator,
  typename T::reference,typename T::pointer,typename T::value_type*>
{
 public:

  typedef typename T::iterator iterator;
  typedef typename T::reference reference;
  typedef typename T::pointer pointer;
  typedef typename T::value_type value_type;
  typedef IteratorBase<iterator,reference,pointer,value_type*> Base;

  IterT(T& t) : Base(t.begin(),t.end()) {}
  IterT(iterator b,iterator e) : Base(b,e) {}
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*!
 * \internal
 * \brief interval d'itérateurs constant
 * \ingroup Collection
 *
 Cette classe gère un couple d'itérateurs qui n'autorisent pas la
 modification des éléments du container.
 */
template<class T>
class ConstIterT
: public IteratorBase<typename T::const_iterator,
  typename T::const_reference,typename T::const_pointer,const typename T::value_type*>
{
 public:

  typedef typename T::const_iterator const_iterator;
  typedef typename T::const_reference const_reference;
  typedef typename T::const_pointer const_pointer;
  typedef typename T::value_type value_type;
  typedef IteratorBase<const_iterator,const_reference,const_pointer,const value_type*> Base;

  ConstIterT(const T& t) : Base(t.begin(),t.end()) {}
  ConstIterT(const_iterator b,const_iterator e) : Base(b,e) {}
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // End namespace Arccore

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif

