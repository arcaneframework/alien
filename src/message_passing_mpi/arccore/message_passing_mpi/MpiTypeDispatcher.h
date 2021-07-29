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
/* MpiTypeDispatcher.h                                         (C) 2000-2020 */
/*                                                                           */
/* Gestion des messages pour un type de données.                             */
/*---------------------------------------------------------------------------*/
#ifndef ARCCORE_MESSAGEPASSINGMPI_MPITYPEDISPATCHER_H
#define ARCCORE_MESSAGEPASSINGMPI_MPITYPEDISPATCHER_H
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include "arccore/message_passing_mpi/MessagePassingMpiGlobal.h"
#include "arccore/message_passing/ITypeDispatcher.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Arccore::MessagePassing::Mpi
{

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

template<typename Type>
class MpiTypeDispatcher
: public ITypeDispatcher<Type>
{
 public:
  MpiTypeDispatcher(IMessagePassingMng* parallel_mng,MpiAdapter* adapter,MpiDatatype* datatype);
  ~MpiTypeDispatcher();
 public:
  void finalize() override {}
  void broadcast(Span<Type> send_buf,Int32 rank) override;
  void allGather(Span<const Type> send_buf,Span<Type> recv_buf) override;
  void allGatherVariable(Span<const Type> send_buf,Array<Type>& recv_buf) override;
  void gather(Span<const Type> send_buf,Span<Type> recv_buf,Int32 rank) override;
  void gatherVariable(Span<const Type> send_buf,Array<Type>& recv_buf,Int32 rank) override;
  void scatterVariable(Span<const Type> send_buf,Span<Type> recv_buf,Int32 root) override;
  void allToAll(Span<const Type> send_buf,Span<Type> recv_buf,Int32 count) override;
  void allToAllVariable(Span<const Type> send_buf,Int32ConstArrayView send_count,
                        Int32ConstArrayView send_index,Span<Type> recv_buf,
                        Int32ConstArrayView recv_count,Int32ConstArrayView recv_index) override;
  Request send(Span<const Type> send_buffer,Int32 rank,bool is_blocked) override;
  Request send(Span<const Type> send_buffer,const PointToPointMessageInfo& message) override;
  Request receive(Span<Type> recv_buffer,Int32 rank,bool is_blocked) override;
  Request receive(Span<Type> recv_buffer,const PointToPointMessageInfo& message) override;
  Type allReduce(eReduceType op,Type send_buf) override;
  void allReduce(eReduceType op,Span<Type> send_buf) override;
 public:
  MpiDatatype* datatype() const { return m_datatype; }
  IMessagePassingMng* messagePassingMng() const { return m_parallel_mng; }
  MpiAdapter* adapter() const { return m_adapter; }
 private:
  IMessagePassingMng* m_parallel_mng;
  MpiAdapter* m_adapter;
  MpiDatatype* m_datatype;
 private:
  void _gatherVariable2(Span<const Type> send_buf,Array<Type>& recv_buf,Integer rank);
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // End namespace Arccore::MessagePassing::Mpi

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif  

