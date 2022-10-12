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

#include <arccore/base/Span.h>
#include "SimpleCSRDistributor.h"

namespace Alien
{

SimpleCSRDistributor::SimpleCSRDistributor(const RedistributorCommPlan* commPlan,
                                           const VectorDistribution& source_distribution)
:
m_comm_plan(commPlan)
{
  const auto& target_distribution = m_comm_plan->distribution();
  const auto me = m_comm_plan->superParallelMng()->commRank();
  const auto dst_me = m_comm_plan->procNum(me);

  typeof(m_src_profile->getNElems()) dst_n_elems = 0;

  for(auto global_row = source_distribution.offset();
       global_row<source_distribution.offset()+source_distribution.localSize();
       global_row++)
  {
    auto target = target_distribution.owner(global_row); // target is and id in tgtParallelMng
    auto src_local_row = source_distribution.globalToLocal(global_row);

    if(target==dst_me)
    {
      auto dst_local_row = m_comm_plan->distribution().globalToLocal(global_row);
      m_src2dst_row_list.push_back({src_local_row,dst_local_row});
      dst_n_elems += m_src_profile->getRowSize(src_local_row);
    }
    else
    {
      auto& comm_info = m_send_comm_info[target];
      comm_info.m_row_list.push_back(src_local_row);
      comm_info.m_n_item += m_src_profile->getRowSize(src_local_row);
    }
  }

  typeof(m_src_profile->getNRows()) ext_dst_n_rows = 0;

  if(dst_me >= 0) // I am in the target parallel manager
  {
    for(auto global_row = target_distribution.offset();
         global_row<target_distribution.offset()+target_distribution.localSize();
         global_row++)
    {
      auto source = source_distribution.owner(global_row); // source is an id in superParallelMng

      if(source!=me)
      {
        auto dst_local_row = target_distribution.globalToLocal(global_row);
        auto& comm_info = m_recv_comm_info[source];
        comm_info.m_row_list.push_back(dst_local_row);
        ext_dst_n_rows++;
      }
    }
  }

  for(auto& [send_id,comm_info] : m_send_comm_info)
  {
    Arccore::MessagePassing::PointToPointMessageInfo message_info(MessageRank(me),MessageRank(send_id),
                                                                  Arccore::MessagePassing::NonBlocking);
    comm_info.m_message_info = message_info;
  }

  for(auto& [recv_id,comm_info] : m_recv_comm_info)
  {
    Arccore::MessagePassing::PointToPointMessageInfo message_info(MessageRank(me),MessageRank(recv_id),
                                                                  Arccore::MessagePassing::NonBlocking);

    comm_info.m_message_info = message_info;
  }

  auto *pm = m_comm_plan->superParallelMng();
  // perform an exchange of sizes
  for(auto& [recv_id,comm_info] : m_recv_comm_info)
  {
    comm_info.m_request = Arccore::MessagePassing::mpReceive(pm,Arccore::Span<size_t>(&comm_info.m_n_item,1),comm_info.m_message_info);
  }

  for(auto& [send_id,comm_info] : m_send_comm_info)
  {
    comm_info.m_request = Arccore::MessagePassing::mpSend(pm,Arccore::Span<size_t>(&comm_info.m_n_item,1),comm_info.m_message_info);
  }

  for(const auto& [recv_id,comm_info] : m_recv_comm_info)
  {
     Arccore::MessagePassing::mpWait(pm,comm_info.m_request);
     dst_n_elems += comm_info.m_n_item;
  }

  for(const auto& [send_id,comm_info] : m_send_comm_info)
  {
     Arccore::MessagePassing::mpWait(pm,comm_info.m_request);
  }

  // create destination profile
  if(dst_me >= 0) {
    m_dst_profile = std::make_shared<Alien::SimpleCSRInternal::CSRStructInfo>(ext_dst_n_rows + m_src2dst_row_list.size(), dst_n_elems);
  }
  // build kcol
  throw Arccore::NotImplementedException(Arccore::TraceInfo(__FILE__,__PRETTY_FUNCTION__,__LINE__));
  // distribute profile cols

  _distribute<int>(1,m_src_profile->cols(),m_dst_profile->cols());
}

template <typename T>
void SimpleCSRDistributor::_distribute(const int bb,const T* src,T* dst)
{

  using ItemType = T;

  for(auto& [send_id,comm_info] : m_send_comm_info)
  {
    comm_info.m_buffer.resize((comm_info.m_n_item*sizeof(ItemType)*bb+sizeof(uint64_t)-1)/sizeof(uint64_t));
  }

  for(auto& [recv_id,comm_info] : m_recv_comm_info)
  {
    comm_info.m_buffer.resize((comm_info.m_n_item*sizeof(ItemType)*bb+sizeof(uint64_t)-1)/sizeof(uint64_t));
  }

  auto* pm = m_comm_plan->superParallelMng();
  // post recv
  for(auto& [recv_id,comm_info] : m_recv_comm_info)
  {
    comm_info.m_request =
    Arccore::MessagePassing::mpReceive(pm,Arccore::Span<uint64_t>(comm_info.m_buffer.data(),comm_info.m_buffer.size()),
                                       comm_info.m_message_info);
  }

  // send rows
  for(auto& [send_id,comm_info] : m_send_comm_info)
  {
    // assemble message
    auto* buffer = (ItemType*)(comm_info.m_buffer.data());
    std::size_t buffer_idx = 0;
    for(const auto &src_row: comm_info.m_row_list)
    {
      for(auto k=m_src_profile->kcol()[src_row]*bb;k<m_src_profile->kcol()[src_row+1]*bb;++k)
      {
        buffer[buffer_idx] = src[k];
        buffer_idx++;
      }
    }
    comm_info.m_request =
    Arccore::MessagePassing::mpSend(pm,Arccore::Span<uint64_t>(comm_info.m_buffer.data(),comm_info.m_buffer.size()),
                                    comm_info.m_message_info);
  }
  // perform direct transfer
  for(const auto& [src_row,dst_row] : m_src2dst_row_list)
  {
    auto k_src = m_src_profile->kcol()[src_row]*bb;
    for(auto k_dst=m_dst_profile->kcol()[dst_row]*bb;k_dst<m_dst_profile->kcol()[dst_row+1]*bb;++k_dst)
    {
      dst[k_dst] = src[k_src];
      k_src++;
    }
    assert(k_src==m_src_profile->kcol()[src_row+1]*bb);
  }

  // wait for recv messages
  for(auto const& [recv_id,comm_info]: m_recv_comm_info)
  {
    Arccore::MessagePassing::mpWait(pm,comm_info.m_request);

    // put received matrix values at the right place
    const auto* buffer = (const ItemType*)(comm_info.m_buffer.data());
    std::size_t buffer_idx = 0;
    for(const auto& dst_row : m_recv_comm_info[recv_id].m_row_list)
    {
      for(auto k=m_dst_profile->kcol()[dst_row]*bb;k<m_dst_profile->kcol()[dst_row+1]*bb;++k)
      {
        dst[k] = buffer[buffer_idx];
        buffer_idx++;
      }
    }
  }

  // finish properly
  for(auto const& [send_id,comm_info] : m_send_comm_info)
  {
    Arccore::MessagePassing::mpWait(pm,comm_info.m_request);
  }

}
template <typename NumT>
void SimpleCSRDistributor::distribute(const SimpleCSRMatrix<NumT>& src, SimpleCSRMatrix<NumT>& dst)
{
  throw Arccore::NotImplementedException(Arccore::TraceInfo(__FILE__,__PRETTY_FUNCTION__,__LINE__));
}
template <typename NumT>
void SimpleCSRDistributor::distribute(const SimpleCSRVector<NumT>& src, SimpleCSRVector<NumT>& dst)
{
  throw Arccore::NotImplementedException(Arccore::TraceInfo(__FILE__,__PRETTY_FUNCTION__,__LINE__));
}

} // namespace Alien
