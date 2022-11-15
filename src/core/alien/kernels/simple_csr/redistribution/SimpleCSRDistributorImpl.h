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
#include <mpi.h>

#include <arccore/base/Span.h>
#include "SimpleCSRDistributor.h"

namespace Alien
{

SimpleCSRDistributor::SimpleCSRDistributor(const RedistributorCommPlan* commPlan,
                                           const VectorDistribution& source_distribution,
                                           const Alien::SimpleCSRInternal::CSRStructInfo* src_profile)
:
m_comm_plan(commPlan),
m_src_profile(src_profile)
{
  const auto me = m_comm_plan->superParallelMng()->commRank();
  const auto dst_me = m_comm_plan->procNum(me);

  // send target distribution to
  // all processors that are not in source comm
  std::vector<int> target_offset;
  const auto src_comm_size = m_comm_plan->superParallelMng()->commSize();

  if(dst_me == 0)
  {
    // TODO: broadcast target_offset from more than on proc

    const auto& target_distribution = m_comm_plan->distribution();
    const auto target_np = m_comm_plan->distribution().parallelMng()->commSize();
    target_offset.resize(target_np+1);
    for(int i=0;i<target_np+1;++i)
    {
      target_offset[i] = target_distribution.offset(i);
    }

    auto size = target_offset.size();

    for(int i=0;i<src_comm_size;++i)
    {
      if(m_comm_plan->procNum(i) < 0)
      {

        // proc i in superParallelMng is not in target so it must have offsets
        Arccore::MessagePassing::PointToPointMessageInfo
          message_info(MessageRank(me),MessageRank(i),Arccore::MessagePassing::MessageTag(444),
                       Arccore::MessagePassing::Blocking);

        Arccore::MessagePassing::mpSend(m_comm_plan->superParallelMng(),
                                        Arccore::Span<size_t>(&size,1),
                                        message_info);
        Arccore::MessagePassing::mpSend(m_comm_plan->superParallelMng(),
                                        Arccore::Span<int>(target_offset.data(),target_offset.size()),
                                        message_info);
      }
    }
  }
  else if(dst_me < 0)
  {
    // I'am not in target distribution so get target_offset
    Arccore::MessagePassing::PointToPointMessageInfo
      message_info(MessageRank(me),MessageRank(),Arccore::MessagePassing::MessageTag(444),
                   Arccore::MessagePassing::Blocking);

    size_t size = 0;
    Arccore::MessagePassing::mpReceive(m_comm_plan->superParallelMng(),
                                       Arccore::Span<size_t>(&size,1),
                                       message_info);
    target_offset.resize(size);
    Arccore::MessagePassing::mpReceive(m_comm_plan->superParallelMng(),
                                       Arccore::Span<int>(target_offset.data(),target_offset.size()),
                                       message_info);

  }

  typeof(m_src_profile->getNElems()) dst_n_elems = 0;

  for(auto global_row = source_distribution.offset();
       global_row<source_distribution.offset()+source_distribution.localSize();
       global_row++)
  {
    auto target = _owner(target_offset,global_row); // target is and id in tgtParallelMng
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
    const auto& target_distribution = m_comm_plan->distribution();

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

  _finishExchange();

  // create destination profile
  if(dst_me >= 0) {
    m_dst_profile = std::make_shared<Alien::SimpleCSRInternal::CSRStructInfo>();
    m_dst_profile->init(ext_dst_n_rows + m_src2dst_row_list.size(),dst_n_elems);
  }

  // build destination kcol
  // exchange row sizes
  _resizeBuffers<int>(1); // comm_info.m_n_item are already computed so buffer sizes are sufficient for row size exchange

  for(auto& [recv_id,comm_info] : m_recv_comm_info)
  {
    comm_info.m_request = Arccore::MessagePassing::mpReceive(pm,Arccore::Span<int>((int*)comm_info.m_buffer.data(),comm_info.m_row_list.size()),comm_info.m_message_info);
  }

  for(auto& [send_id,comm_info] : m_send_comm_info)
  {
    auto* buffer = (int*)(comm_info.m_buffer.data());
    std::size_t buffer_idx = 0;
    assert(comm_info.m_row_list.size()<=comm_info.m_n_item); // check that buffer is large enough
    for(const auto &src_row: comm_info.m_row_list)
    {
      buffer[buffer_idx] = m_src_profile->getRowSize(src_row);
      buffer_idx++;
    }
    comm_info.m_request = Arccore::MessagePassing::mpSend(pm,Arccore::Span<int>((int*)comm_info.m_buffer.data(),comm_info.m_row_list.size()),comm_info.m_message_info);
  }

  std::vector<int> row_size(ext_dst_n_rows+m_src2dst_row_list.size(),0);

  // self rows (if exist)
  for(const auto& [src_row,dst_row] : m_src2dst_row_list)
  {
    row_size[dst_row] = m_src_profile->getRowSize(src_row);
  }
  // wait for recv messages
  for(auto const& [recv_id,comm_info]: m_recv_comm_info)
  {
    Arccore::MessagePassing::mpWait(pm,comm_info.m_request);

    const auto* buffer = (const int*)(comm_info.m_buffer.data());
    std::size_t buffer_idx = 0;
    for(const auto& dst_row : m_recv_comm_info[recv_id].m_row_list)
    {
      row_size[dst_row] = buffer[buffer_idx];
      buffer_idx++;
    }
  }
  _finishExchange();

  if(dst_me >= 0) {
    auto* kcol = m_dst_profile->kcol();
    kcol[0] = 0;
    for(int i=1;i<m_dst_profile->getNRows()+1;++i) {
      kcol[i] = kcol[i-1]+row_size[i-1];
    }
  }

  // distribute profile cols
  _distribute<int>(1,m_src_profile->cols(),dst_me >= 0 ? m_dst_profile->cols(): nullptr);
}

template <typename T>
void SimpleCSRDistributor::_distribute(const int bb,const T* src,T* dst)
{

  using ItemType = T;

  _resizeBuffers<T>(bb);

  auto* pm = m_comm_plan->superParallelMng();
  // post recv
  for(auto& [recv_id,comm_info] : m_recv_comm_info)
  {
    comm_info.m_request =
    Arccore::MessagePassing::mpReceive(pm,Arccore::Span<T>((T*)comm_info.m_buffer.data(),comm_info.m_n_item),
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
    Arccore::MessagePassing::mpSend(pm,Arccore::Span<T>((T*)comm_info.m_buffer.data(),comm_info.m_n_item),
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

  _finishExchange();

}
template <typename NumT>
void SimpleCSRDistributor::distribute(const SimpleCSRMatrix<NumT>& src, SimpleCSRMatrix<NumT>& dst)
{
  const auto me = m_comm_plan->superParallelMng()->commRank();
  const auto dst_me = m_comm_plan->procNum(me);

  if(dst_me >= 0) {
    // I am in the target parallel manager
    // fill dst profile with a copy of m_dst_profile
    auto& profile = dst.internal().getCSRProfile();
    profile.init(m_dst_profile->getNRows(),m_dst_profile->getNElems());
    dst.allocate();

    for (int i = 0; i < profile.getNRows() + 1; ++i) {
      profile.kcol()[i] = m_dst_profile->kcol()[i];
    }
    for (int k = 0; k < profile.getNElems(); ++k) {
      profile.cols()[k] = m_dst_profile->cols()[k];
    }
  }

  if(src.block()) {
    _distribute(src.block()->sizeX()*src.block()->sizeY(), src.data(), dst.data());
  }
  else if(src.vblock()) {
     throw Arccore::NotImplementedException(Arccore::TraceInfo(__FILE__,__PRETTY_FUNCTION__,__LINE__));
  }
  else {
    _distribute(1, src.data(), dst.data());
  }

#if 0
  if(dst_me == 0)
  {
    const auto& profile = dst.internal().getCSRProfile();
    for (int i = 0; i < profile.getNRows(); ++i)
    {
      std::cout << i ;
      for (int k = profile.kcol()[i]; k < profile.kcol()[i+1]; ++k)
      {
        std::cout << " [" << profile.cols()[k] << " " << dst.data()[k] << "]";
      }
      std::cout << std::endl;
    }
  }
#endif
}

template <typename NumT>
void SimpleCSRDistributor::distribute(const SimpleCSRVector<NumT>& src, SimpleCSRVector<NumT>& dst)
{
  throw Arccore::NotImplementedException(Arccore::TraceInfo(__FILE__,__PRETTY_FUNCTION__,__LINE__));
}

template <typename T>
void SimpleCSRDistributor::_resizeBuffers(const int bb)
{
  for(auto& [send_id,comm_info] : m_send_comm_info)
  {
    comm_info.m_buffer.resize((comm_info.m_n_item*sizeof(T)*bb+sizeof(uint64_t)-1)/sizeof(uint64_t));
  }

  for(auto& [recv_id,comm_info] : m_recv_comm_info)
  {
    comm_info.m_buffer.resize((comm_info.m_n_item*sizeof(T)*bb+sizeof(uint64_t)-1)/sizeof(uint64_t));
  }
}

void SimpleCSRDistributor::_finishExchange()
{
  auto* pm = m_comm_plan->superParallelMng();
  // finish properly
  for(auto const& [send_id,comm_info] : m_send_comm_info)
  {
    Arccore::MessagePassing::mpWait(pm,comm_info.m_request);
  }
}

// T must be an integer signed type
template<typename T>
T SimpleCSRDistributor::_owner(const std::vector<T>& offset, T global_id)
{
  if(global_id >= offset.back())
    return -1;

  auto min = offset.size()-offset.size(); // just for the right auto type
  auto max = offset.size();

  while(max-min>1) {
    auto mid = (max - min) / 2 + min;
    if (global_id < offset[mid]) {
      max = mid;
    }
    else {
      min = mid;
    }
  }

  return min;
}

} // namespace Alien
