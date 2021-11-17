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
/* ITypeDispatcher.h                                           (C) 2000-2020 */
/*                                                                           */
/* Gestion des messages pour un type de données.                             */
/*---------------------------------------------------------------------------*/
#ifndef ARCCORE_MESSAGEPASSING_ITYPEDISPATCHER_H
#define ARCCORE_MESSAGEPASSING_ITYPEDISPATCHER_H
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include "arccore/message_passing/MessagePassingGlobal.h"
#include "arccore/collections/CollectionsGlobal.h"
#include "arccore/base/BaseTypes.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Arccore::MessagePassing
{

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*!
 * \internal
 * \brief Gestion des messages parallèles pour le type \a Type.
 */
template<class Type>
class ITypeDispatcher
{
 public:
  virtual ~ITypeDispatcher() {}
  virtual void finalize() =0;
 public:
  virtual void broadcast(Span<Type> send_buf,Int32 rank) =0;
  virtual void allGather(Span<const Type> send_buf,Span<Type> recv_buf) =0;
  virtual void allGatherVariable(Span<const Type> send_buf,Array<Type>& recv_buf) =0;
  virtual void gather(Span<const Type> send_buf,Span<Type> recv_buf,Int32 rank) =0;
  virtual void gatherVariable(Span<const Type> send_buf,Array<Type>& recv_buf,Int32 rank) =0;
  virtual void scatterVariable(Span<const Type> send_buf,Span<Type> recv_buf,Int32 root) =0;
  virtual void allToAll(Span<const Type> send_buf,Span<Type> recv_buf,Int32 count) =0;
  virtual void allToAllVariable(Span<const Type> send_buf,ConstArrayView<Int32> send_count,
                                ConstArrayView<Int32> send_index,Span<Type> recv_buf,
                                ConstArrayView<Int32> recv_count,ConstArrayView<Int32> recv_index) =0;
  virtual Request send(Span<const Type> send_buffer,Int32 rank,bool is_blocked) =0;
  virtual Request send(Span<const Type> send_buffer,const PointToPointMessageInfo& message) =0;
  virtual Request receive(Span<Type> recv_buffer,Int32 rank,bool is_blocked) =0;
  virtual Request receive(Span<Type> recv_buffer,const PointToPointMessageInfo& message) =0;
  virtual Type allReduce(eReduceType op,Type send_buf) =0;
  virtual void allReduce(eReduceType op,Span<Type> send_buf) =0;
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // End namespace Arccore::MessagePassing

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif  
