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



#include <alien/kernels/sycl/data/SYCLEnvInternal.h>
#include <alien/kernels/sycl/data/SYCLEnv.h>

#include <alien/kernels/sycl/data/SYCLVectorInternal.h>
#include <alien/kernels/sycl/data/SYCLVector.h>

#include <alien/kernels/sycl/data/SYCLBEllPackInternal.h>
#include <alien/kernels/sycl/data/SYCLBEllPackMatrix.h>

/*---------------------------------------------------------------------------*/

namespace Alien
{

  SYCLEnv::SYCLEnv()
  {
    m_internal = new SYCLInternal::EnvInternal ;
  }


  SYCLEnv::~SYCLEnv()
  {
    delete m_internal ;
  }

  SYCLEnv* SYCLEnv::m_instance = nullptr ;

  SYCLEnv* SYCLEnv::instance()
  {
    if(!m_instance)
      m_instance = new SYCLEnv ;
    return m_instance ;
  }

  std::size_t SYCLEnv::maxNumGroups() {
    return m_internal->maxNumGroups() ;
  }

  std::size_t SYCLEnv::maxWorkGroupSize() {
    return m_internal->maxWorkGroupSize() ;
  }

  std::size_t SYCLEnv::maxNumThreads() {
    return m_internal->maxNumThreads() ;
  }

    template <int BlockSize, typename IndexT>
    void BEllPackStructInfo<BlockSize,IndexT>::computeBlockRowOffset(std::vector<int>& block_row_offset,
                                                                     std::size_t nrows,
                                                                     int const* kcol)
    {
      std::size_t block_nrows = BEllPackStructInfo<BlockSize,IndexT>::nbBlocks(nrows) ;
      block_row_offset.resize(block_nrows+1) ;
      int offset = 0 ;
      for(std::size_t ib=0;ib<block_nrows;++ib)
      {
        block_row_offset[ib] = offset ;
        int max_row_size = 0 ;
        for(std::size_t i=0;i<std::min(BlockSize,int(nrows-ib*BlockSize));++i)
        {
          int irow = ib*BlockSize+i;
          int row_size = kcol[irow+1]-kcol[irow];
          //std::cout<<"ROW["<<ib<<","<<irow<<"] :"<<kcol[irow]<<","<<kcol[irow+1]<<" row_size="<<row_size<<" offset="<<offset<<std::endl ;
          max_row_size = std::max(max_row_size,row_size) ;
        }
        offset += max_row_size ;
      }
      block_row_offset[block_nrows] = offset ;
    }

    template <int BlockSize, typename IndexT>
    SYCLInternal::StructInfoInternal<BlockSize,IndexT>::
    StructInfoInternal(std::size_t nrows,
                       std::size_t nnz,
                       std::size_t block_nrows,
                       std::size_t block_nnz,
                       int const* h_kcol,
                       int const* h_cols,
                       int const* h_block_row_offset)
    : m_nrows(nrows)
    , m_nnz(nnz)
    , m_block_nrows(block_nrows)
    , m_block_nnz(block_nnz)
    , m_h_kcol(h_kcol,h_kcol+nrows+1)
    , m_h_cols(h_cols,h_cols+nnz)
    , m_h_block_cols(block_nnz*block_size)
    , m_block_row_offset(h_block_row_offset,cl::sycl::range<1>(m_block_nrows+1))
    , m_block_cols(m_h_block_cols.data(),cl::sycl::range<1>(m_block_nnz*block_size))
    , m_kcol(m_h_kcol.data(),cl::sycl::range<1>(m_nrows+1))
    {
      auto env = SYCLEnv::instance() ;

      auto& queue = env->internal()->queue() ;

      auto num_groups = env->internal()->maxNumGroups();

      auto max_work_group_size = env->internal()->maxWorkGroupSize();

      // building the best number of global thread
      auto total_threads = num_groups * block_size;

      //std::copy(h_kcol,h_kcol+nrows+1,m_h_kcol.begin()) ;
      //std::copy(h_cols,h_cols+nnz,m_h_cols.begin()) ;

      /*
      std::cout<<"CHECK COLS VALUES INPUT"<<m_h_kcol.size()<<" "<<m_h_cols.size()<<" "<<m_h_block_cols.size()<<std::endl ;
      for(std::size_t i=0;i<m_nrows;++i)
      {
        std::cout<<"ROW_SIZE["<<i<<"]="<<m_h_kcol[i+1]-m_h_kcol[i]<<" "<<h_block_row_offset[i/block_size+1]-h_block_row_offset[i/block_size]<<std::endl ;
        for(int k = m_h_kcol[i];k<m_h_kcol[i+1];++k)
        {
          std::cout<<"("<<k<<","<<m_h_cols[k]<<");";
        }
        std::cout<<std::endl ;
      }*/

      //m_block_row_offset.set_final_data(nullptr);
      //m_block_cols.set_final_data(nullptr);
      //m_kcol.set_final_data(nullptr);

      //std::vector<int> row_size(m_nrows) ;
      //std::vector<int> block_row_size(m_nrows) ;
      //std::vector<int> row_begin(m_nrows) ;
      //std::vector<int> row_end(m_nrows) ;
      //std::vector<int> row_first_cols(m_nrows) ;

      {

        //IndexBufferType row_size_buffer(row_size.data(),cl::sycl::range<1>(m_nrows)) ;
        /*
        IndexBufferType block_row_size_buffer(block_row_size.data(),cl::sycl::range<1>(m_nrows)) ;
        IndexBufferType row_begin_buffer(row_begin.data(),cl::sycl::range<1>(m_nrows)) ;
        IndexBufferType row_end_buffer(row_end.data(),cl::sycl::range<1>(m_nrows)) ;
        IndexBufferType row_first_cols_buffer(row_first_cols.data(),cl::sycl::range<1>(m_nrows)) ;
        */

        //IndexBufferType m_block_row_offset(h_block_row_offset,cl::sycl::range<1>(m_block_nrows+1)) ;
        //IndexBufferType m_block_cols(m_h_block_cols.data(),cl::sycl::range<1>(m_block_nnz*block_size)) ;
        //IndexBufferType m_kcol(m_h_kcol.data(),cl::sycl::range<1>(m_nrows+1)) ;

        IndexBufferType cols_buffer(m_h_cols.data(),cl::sycl::range<1>(m_nnz)) ;

        auto nrows = m_nrows ;


          queue.submit([&](cl::sycl::handler& cgh)
                   {

                     //auto access_rx = row_size_buffer.template get_access<cl::sycl::access::mode::read_write>(cgh);
                     /*
                     auto access_brs = block_row_size_buffer.template get_access<cl::sycl::access::mode::read_write>(cgh);
                     auto access_begin = row_begin_buffer.template get_access<cl::sycl::access::mode::read_write>(cgh);
                     auto access_end   = row_end_buffer.template get_access<cl::sycl::access::mode::read_write>(cgh);
                     auto access_first_cols = row_first_cols_buffer.template get_access<cl::sycl::access::mode::read_write>(cgh);
                     */
                     auto access_kcol_buffer      = m_kcol.template get_access<cl::sycl::access::mode::read>(cgh);
                     auto access_block_row_offset = m_block_row_offset.template get_access<cl::sycl::access::mode::read>(cgh);

                     auto access_cols_buffer      = cols_buffer.template get_access<cl::sycl::access::mode::read>(cgh);
                     auto access_block_cols       = m_block_cols.template get_access<cl::sycl::access::mode::read_write>(cgh);


                     cgh.parallel_for<class vector_rs>(cl::sycl::range<1>{total_threads}, [=] (cl::sycl::item<1> itemId)
                                                        {
                                                           auto id = itemId.get_id(0);

                                                           for (auto i = id; i < nrows; i += itemId.get_range()[0])
                                                           {
                                                             auto block_id = i/block_size ;
                                                             auto local_id = i%block_size ;

                                                             int begin              = access_kcol_buffer[i] ;
                                                             int end                = access_kcol_buffer[i+1] ;
                                                             int row_size           = end - begin ;

                                                             int block_row_offset   = access_block_row_offset[block_id]*block_size ;
                                                             auto block_row_size = access_block_row_offset[block_id+1]-access_block_row_offset[block_id] ;

                                                             //access_rx[i] = row_size;
                                                              /*
                                                              access_brs[i] = block_row_size ;
                                                              access_begin[i] = begin ;
                                                              access_end[i] = end ;
                                                              access_first_cols[i] = access_cols_buffer[begin] ;
                                                              */
                                                              for(int k=begin;k<end;++k)
                                                              {
                                                                access_block_cols[block_row_offset+(k-begin)*block_size+local_id] = access_cols_buffer[k] ;
                                                              }
                                                              for(int k=row_size;k<block_row_size;++k)
                                                              {
                                                                access_block_cols[block_row_offset+k*block_size+local_id] = 0 ;
                                                              }

                                                           }
                                                        });
                   });
      }
      /*
      std::cout<<"CHECK COLS VALUES"<<block_nnz*block_size<<std::endl ;

      auto block_cols = m_block_cols.template get_access<cl::sycl::access::mode::read>();
      for(std::size_t i=0;i<m_nrows;++i)
      {
        //std::cout<<"ROW_SIZE["<<i<<"]="<<row_size[i]<<" "<<block_row_size[i]<<" "<<row_begin[i]<<" "<<row_end[i]<<" "<<row_first_cols[i]<<std::endl ;
        auto block_id = i/block_size ;
        auto local_id = i%block_size ;
        auto block_row_offset = h_block_row_offset[block_id] ;
        auto rsize = m_h_kcol[i+1]-m_h_kcol[i] ;
        std::cout<<"BLK COLS["<<i<<"]: row_size="<<rsize<<" : ";
        for(std::size_t k=0;k<rsize;++k)
        {
          //std::cout<<"["<<k<<","<<m_h_block_cols[block_row_offset+k*block_size+local_id]<<"],";
          std::cout<<"["<<k<<","<<block_cols[block_row_offset+k*block_size+local_id]<<"],";
        }
        std::cout<<std::endl ;
      }*/

      /*
      {
        IndexBufferType row_size_buffer(row_size.data(),cl::sycl::range<1>(m_nrows)) ;
        IndexBufferType block_row_size_buffer(block_row_size.data(),cl::sycl::range<1>(m_nrows)) ;

        IndexBufferType m_block_row_offset(h_block_row_offset,cl::sycl::range<1>(m_block_nrows+1)) ;
        //IndexBufferType m_block_cols(h_block_cols,cl::sycl::range<1>(m_block_nnz*block_size)) ;
        IndexBufferType m_kcol(h_kcol,cl::sycl::range<1>(m_nrows+1)) ;

        //IndexBufferType cols_buffer(h_cols,cl::sycl::range<1>(m_nnz)) ;

        // COMPUTE COLS
        queue.submit([&](cl::sycl::handler& cgh)
                 {
                   auto access_kcol_buffer      = m_kcol.template get_access<cl::sycl::access::mode::read>(cgh);
                   //auto access_cols_buffer      = cols_buffer.template get_access<cl::sycl::access::mode::read>(cgh);

                   auto access_block_row_offset = m_block_row_offset.template get_access<cl::sycl::access::mode::read>(cgh);
                   //auto access_block_cols       = m_block_cols.template get_access<cl::sycl::access::mode::read_write>(cgh);

                   auto access_row_size         = row_size_buffer.template get_access<cl::sycl::access::mode::read_write>(cgh);
                   auto access_block_row_size   = block_row_size_buffer.template get_access<cl::sycl::access::mode::read_write>(cgh);

                   cgh.parallel_for<class vector_axpy>(cl::sycl::range<1>{total_threads}, [&] (cl::sycl::item<1> item_id)
                   //cgh.parallel_for<class vector_axpy>(cl::sycl::nd_range<1>{cl::sycl::range<1>{total_threads},
                   //                                                          cl::sycl::range<1>{block_size}},[=](cl::sycl::nd_item<1> item_id)
                                                     {

                                                        auto global_id = item_id.get_id(0);
                                                        auto block_id = global_id/block_size ;
                                                        auto local_id = global_id%block_size ;
                                                        //auto local_id  = item_id.get_local_id(0);
                                                        //auto block_id  = item_id.get_group(0) ;
                                                        //auto global_id = item_id.get_global_id(0);
                                                        for (auto i = global_id; i < m_nrows; i += total_threads)
                                                        {
                                                          int block_row_offset   = access_block_row_offset[block_id]*block_size ;
                                                          int block_row_size     = access_block_row_offset[block_id+1] - access_block_row_offset[block_id] ;

                                                          int begin              = access_kcol_buffer[i] ;
                                                          int end                = access_kcol_buffer[i+1] ;
                                                          int row_size           = end - begin ;

                                                          access_row_size[i]     = row_size ;
                                                          access_block_row_size[i] = block_row_size ;

                                                          for(int k=begin;k<end;++k)
                                                          {
                                                            //access_block_cols[block_row_offset+(k-begin)*block_size+local_id] = access_cols_buffer[k] ;
                                                          }
                                                          for(int k=row_size;k<block_row_size;++k)
                                                          {
                                                            //access_block_cols[block_row_offset+k*block_size+local_id] = 0 ;
                                                          }
                                                          //block_id += num_groups ;
                                                        }
                                                     });
                 }) ;
        //auto block_cols = m_block_cols.template get_access<cl::sycl::access::mode::read>();
        for(std::size_t i=0;i<block_nnz*block_size;++i)
        {
          //std::cout<<"BLK COLS["<<i<<"]="<<block_cols[i] <<std::endl ;
        }

      }*/
    }

    template <int BlockSize, typename IndexT>
    BEllPackStructInfo<BlockSize,IndexT>::BEllPackStructInfo(std::size_t nrows,
                                                             int const* kcol,
                                                             int const* cols,
                                                             int const* h_block_row_offset)

    : m_nrows(nrows)
    , m_nnz(kcol[nrows])
    , m_block_nrows(BEllPackStructInfo<BlockSize,IndexT>::nbBlocks(nrows))
    , m_block_nnz(h_block_row_offset[m_block_nrows])
    //, m_h_kcol{kcol,kcol+m_nrows+1}
    //, m_h_cols{cols,cols+m_nnz}
    //, m_h_block_cols(m_block_nnz*block_size)
    {
      alien_debug([&] {
                           cout() << "COMPUTE BELLPACK PROFILE";
                           cout() << "       NROWS : "<<m_nrows;
                           cout() << "       NNZ   : "<<m_nnz;
                           cout() << " BLOCK NROWS : "<<m_block_nrows;
                           cout()  <<" BLOCK NNZ   : "<<m_block_nnz;
                      });

      //m_h_block_cols.assign(m_block_nnz*block_size,-1) ;
      m_internal = new InternalType{m_nrows,
                                    m_nnz,
                                    m_block_nrows,
                                    m_block_nnz,
                                    kcol,
                                    cols,
                                    h_block_row_offset} ;

    }

  namespace SYCLInternal
  {
    template <typename ValueT, int BlockSize>
    MatrixInternal<ValueT, BlockSize>::MatrixInternal(ProfileType const* profile)
    : m_profile(profile)
    , m_h_values(profile->getBlockNnz()*block_size)
    , m_values(m_h_values.data(),cl::sycl::range<1>(profile->getBlockNnz()*block_size))
    {
      //m_values.set_final_data(nullptr);
    }

    template <typename ValueT, int BlockSize>
    bool MatrixInternal<ValueT, BlockSize>::setMatrixValues(ValueT const* values)
    {

      auto env                 = SYCLEnv::instance() ;
      auto& queue              = env->internal()->queue() ;
      auto num_groups          = env->internal()->maxNumGroups();
      auto max_work_group_size = env->internal()->maxWorkGroupSize();
      auto total_threads       = num_groups * block_size;

      auto nrows     = m_profile->getNRows() ;
      auto nnz       = m_profile->getNnz() ;
      auto block_nnz = m_profile->getBlockNnz() ;

      auto internal_profile  = m_profile->internal() ;
      auto& kcol             = internal_profile->getKCol() ;
      auto& block_row_offset = internal_profile->getBlockRowOffset() ;


      std::vector<ValueType> h_values(values,values+nnz) ;
      //std::vector<ValueType> h_block_values(block_nnz*block_size) ;
      /*
      std::cout<<"COMPUTE VALUES"<< nrows<<" "<<nnz<<" "<<m_profile->internal()->m_block_nrows<<" "<<m_profile->internal()->m_block_nnz<<std::endl ;

      //std::vector<ValueType> h_values{values,values+nnz} ;
      std::cout<<"CHECK VALUES"<<nnz<<" "<<m_values.get_count()<<std::endl ;
      for(std::size_t i=0;i<nnz;++i)
      {
        std::cout<<"ORI Matrix["<<i<<"]="<<h_values[i]<<std::endl ;
      }*/

      {
        ValueBufferType values_buffer(h_values.data(),cl::sycl::range<1>(nnz)) ;
        //ValueBufferType block_values_buffer(h_block_values.data(),cl::sycl::range<1>(block_nnz*block_size)) ;
        // COMPUTE COLS
        queue.submit([&](cl::sycl::handler& cgh)
                     {
                       auto access_kcol_buffer      = internal_profile->getKCol().template get_access<cl::sycl::access::mode::read>(cgh);
                       auto access_block_row_offset = internal_profile->getBlockRowOffset().template get_access<cl::sycl::access::mode::read>(cgh);
                       auto access_values_buffer    = values_buffer.template get_access<cl::sycl::access::mode::read>(cgh);
                       auto access_block_values     = m_values.template get_access<cl::sycl::access::mode::read_write>(cgh);

                       //cgh.parallel_for<class vector_axpy>(cl::sycl::nd_range<1>{cl::sycl::range<1>{total_threads},cl::sycl::range<1>{block_size}},[=](cl::sycl::nd_item<1> item_id)
                       cgh.parallel_for<class vector_values>(cl::sycl::range<1>{total_threads},
                                                             [=] (cl::sycl::item<1> item_id)
                                                             {
                                                               auto id = item_id.get_id(0);
                                                               //auto local_id  = item_id.get_local_id(0);
                                                               //auto block_id  = item_id.get_group(0) ;
                                                               //auto global_id = item_id.get_global_id(0);

                                                               for (auto i = id; i < nrows; i += item_id.get_range()[0])
                                                               {
                                                                  auto block_id = i/block_size ;
                                                                  auto local_id = i%block_size ;

                                                                  auto begin              = access_kcol_buffer[i] ;
                                                                  auto end                = access_kcol_buffer[i+1] ;
                                                                  auto row_size           = end - begin ;

                                                                  int block_row_offset   = access_block_row_offset[block_id]*block_size ;
                                                                  auto block_row_size    = access_block_row_offset[block_id+1]-access_block_row_offset[block_id] ;

                                                                  for(int k=begin;k<end;++k)
                                                                  {
                                                                    access_block_values[block_row_offset+(k-begin)*block_size+local_id] = access_values_buffer[k] ;
                                                                  }
                                                                  for(int k=row_size;k<block_row_size;++k)
                                                                  {
                                                                    access_block_values[block_row_offset+k*block_size+local_id] = 0 ;
                                                                  }
                                                               }
                                                            });
                     }) ;
      }
      /*
      std::cout<<"CHECK MATRIX VALUES"<<block_nnz*block_size<<std::endl ;

      auto h_block_values     = m_values.template get_access<cl::sycl::access::mode::read>();
      auto h_kcol             = kcol.template get_access<cl::sycl::access::mode::read>();
      auto h_block_row_offset = kcol.template get_access<cl::sycl::access::mode::read>();
      for(std::size_t i=0;i<nrows;++i)
      {
        //std::cout<<"ROW_SIZE["<<i<<"]="<<row_size[i]<<" "<<block_row_size[i]<<" "<<row_begin[i]<<" "<<row_end[i]<<" "<<row_first_cols[i]<<std::endl ;
        auto block_id = i/block_size ;
        auto local_id = i%block_size ;
        auto block_row_offset = h_block_row_offset[block_id] ;
        auto rsize = h_kcol[i+1]-h_kcol[i] ;
        std::cout<<"BLK VALUES["<<i<<"]: row_size="<<rsize<<" : ";
        for(std::size_t k=0;k<rsize;++k)
        {
          std::cout<<"["<<k<<","<<h_block_values[block_row_offset+k*block_size+local_id]<<"],";
        }
        std::cout<<std::endl ;
      }*/
        // COMPUTE VALUES
        /*
        queue.submit([&](cl::sycl::handler& cgh)
                 {
                   auto access_kcol_buffer      = m_profile->internal()->getKCol().template get_access<cl::sycl::access::mode::read>(cgh);
                   auto access_block_row_offset = m_profile->internal()->getBlockRowOffset().template get_access<cl::sycl::access::mode::read>(cgh);
                   auto access_values_buffer    = values_buffer.template get_access<cl::sycl::access::mode::read>(cgh);
                   auto access_values           = block_values_buffer.template get_access<cl::sycl::access::mode::read_write>(cgh);

                   cgh.parallel_for<class compute_values>(cl::sycl::nd_range<1>{cl::sycl::range<1>{total_threads},
                                                                                cl::sycl::range<1>{block_size}},
                                                          [=](cl::sycl::nd_item<1> item_id)
                                                          {
                                                            auto local_id  = item_id.get_local_id(0);
                                                            auto block_id  = item_id.get_group(0) ;
                                                            auto global_id = item_id.get_global_id(0);

                                                            for (auto i = global_id; i < nrows; i += item_id.get_global_range()[0])
                                                            {
                                                              int block_row_offset   = access_block_row_offset[block_id]*block_size ;
                                                              int block_row_size     = access_block_row_offset[block_id+1] - access_block_row_offset[block_id] ;

                                                              int begin              = access_kcol_buffer[i] ;
                                                              int end                = access_kcol_buffer[i+1] ;
                                                              int row_size           = end-begin ;

                                                              for(int k=begin;k<end;++k)
                                                              {
                                                                access_values[block_row_offset+(k-begin)*block_size+local_id] = access_values_buffer[k] ;
                                                              }
                                                              for(int k=row_size;k<block_row_size;++k)
                                                              {
                                                                access_values[block_row_offset+k] = 0. ;
                                                              }
                                                              block_id += item_id.get_global_range()[0] ;
                                                          }
                                                      });
                 });

      }

      std::cout<<"CHECK VALUES"<<h_block_values.size()<<std::endl ;
      //auto h_v = m_values.template get_access<cl::sycl::access::mode::read>();
      for(std::size_t i=0;i<block_nnz*block_size;++i)
      {
        std::cout<<"Matrix["<<i<<"]="<<h_block_values[i] <<std::endl ;
      }*/

      return true ;
    }

    template <typename ValueT, int BlockSize>
    void MatrixInternal<ValueT, BlockSize>::mult(ValueBufferType& x, ValueBufferType& y, cl::sycl::queue& queue) const
    {

      //std::cout<<"MatrixInternal::mult"<<std::endl ;

      auto device = queue.get_device();

      auto num_groups = queue.get_device().get_info<cl::sycl::info::device::max_compute_units>();
      // getting the maximum work group size per thread
      auto max_work_group_size = queue.get_device().get_info<cl::sycl::info::device::max_work_group_size>();
      // building the best number of global thread
      auto total_threads = num_groups * block_size;

      auto nrows = m_profile->getNRows() ;
      auto nnz   = m_profile->getNnz() ;

      auto internal_profile  = m_profile->internal() ;
      auto& kcol             = internal_profile->getKCol() ;
      auto& block_row_offset = internal_profile->getBlockRowOffset() ;
      auto& block_cols       = internal_profile->getBlockCols() ;
      {
        // COMPUTE VALUES
        queue.submit([&](cl::sycl::handler& cgh)
                 {
                   auto access_block_row_offset = block_row_offset.template get_access<cl::sycl::access::mode::read>(cgh);
                   auto access_cols             = block_cols.template get_access<cl::sycl::access::mode::read>(cgh);
                   auto access_values           = m_values.template get_access<cl::sycl::access::mode::read>(cgh);


                   auto access_x                = x.template get_access<cl::sycl::access::mode::read>(cgh);
                   auto access_y                = y.template get_access<cl::sycl::access::mode::read_write>(cgh);


                   //cl::sycl::nd_range<1> r{cl::sycl::range<1>{total_threads},cl::sycl::range<1>{block_size}};
                   //cgh.parallel_for<class compute_mult>(r, [&](cl::sycl::nd_item<1> item_id)
                   cgh.parallel_for<class compute_mult>(cl::sycl::range<1>{total_threads},
                                                        [=] (cl::sycl::item<1> item_id)
                                                        {
                                                          auto id = item_id.get_id(0);
                                                          //auto local_id  = item_id.get_local_id(0);
                                                          //auto block_id  = item_id.get_group(0) ;
                                                          //auto global_id = item_id.get_global_id(0);

                                                          for (auto i = id; i < nrows; i += item_id.get_range()[0])
                                                          {
                                                             auto block_id = i/block_size ;
                                                             auto local_id = i%block_size ;

                                                             int block_row_offset   = access_block_row_offset[block_id]*block_size ;
                                                             auto block_row_size    = access_block_row_offset[block_id+1]-access_block_row_offset[block_id] ;

                                                             ValueType value = 0. ;
                                                             for(int j=0;j<block_row_size;++j)
                                                             {
                                                               auto k = block_row_offset+j*block_size+local_id ;
                                                                value += access_values[k]* access_x[access_cols[k]] ;
                                                             }
                                                             access_y[i] = value ;
                                                          }
                                                      });
                 });
      }
      /*
      auto h_y = y.template get_access<cl::sycl::access::mode::read>();
      for(std::size_t i=0;i<nrows;++i)
      {
        std::cout<<"Y["<<i<<"]="<<h_y[i] <<std::endl ;
      }*/
      //std::cout<<"Fin MatrixInternal::mult"<<std::endl ;
    }

    template <typename ValueT, int BlockSize>
    void MatrixInternal<ValueT, BlockSize>::mult(ValueBufferType& x, ValueBufferType& y) const
    {
      this->mult(x,y,SYCLEnv::instance()->internal()->queue()) ;
    }


    template <typename ValueT, int BlockSize>
    void MatrixInternal<ValueT, BlockSize>::computeInvDiag(ValueBufferType& y, cl::sycl::queue& queue) const
    {

      auto device = queue.get_device();

      auto num_groups = queue.get_device().get_info<cl::sycl::info::device::max_compute_units>();
      // getting the maximum work group size per thread
      auto max_work_group_size = queue.get_device().get_info<cl::sycl::info::device::max_work_group_size>();
      // building the best number of global thread
      auto total_threads = num_groups * block_size;

      auto nrows = m_profile->getNRows() ;
      auto nnz   = m_profile->getNnz() ;

      auto internal_profile  = m_profile->internal() ;
      auto& kcol             = internal_profile->getKCol() ;
      auto& block_row_offset = internal_profile->getBlockRowOffset() ;
      auto& block_cols       = internal_profile->getBlockCols() ;
      {
        // COMPUTE VALUES
        queue.submit([&](cl::sycl::handler& cgh)
                     {
                       auto access_block_row_offset = block_row_offset.template get_access<cl::sycl::access::mode::read>(cgh);
                       auto access_cols             = block_cols.template get_access<cl::sycl::access::mode::read>(cgh);
                       auto access_values           = m_values.template get_access<cl::sycl::access::mode::read>(cgh);
                       auto access_y                = y.template get_access<cl::sycl::access::mode::read_write>(cgh);


                       cgh.parallel_for<class compute_mult>(cl::sycl::range<1>{total_threads},
                                                            [=] (cl::sycl::item<1> item_id)
                                                            {
                                                              auto id = item_id.get_id(0);
                                                              //auto local_id  = item_id.get_local_id(0);
                                                              //auto block_id  = item_id.get_group(0) ;
                                                              //auto global_id = item_id.get_global_id(0);

                                                              for (auto i = id; i < nrows; i += item_id.get_range()[0])
                                                              {
                                                                 auto block_id = i/block_size ;
                                                                 auto local_id = i%block_size ;

                                                                 int block_row_offset   = access_block_row_offset[block_id]*block_size ;
                                                                 auto block_row_size    = access_block_row_offset[block_id+1]-access_block_row_offset[block_id] ;
                                                                 for(int j=0;j<block_row_size;++j)
                                                                 {
                                                                   auto k = block_row_offset+j*block_size+local_id ;
                                                                   if((access_cols[k])==int(i) && (access_values[k]!=0) )
                                                                     access_y[i] = 1./access_values[k] ;
                                                                 }
                                                              }
                                                            });
                     });
      }
    }


    template <typename ValueT, int BlockSize>
    void MatrixInternal<ValueT, BlockSize>::computeInvDiag(ValueBufferType& y) const
    {
      this->computeInvDiag(y,SYCLEnv::instance()->internal()->queue()) ;
    }
  }


  template <typename ValueT>
  SYCLBEllPackMatrix<ValueT>::~SYCLBEllPackMatrix ()
  {
    delete m_profile1024 ;
    delete m_matrix1024 ;
  }

  template <typename ValueT>
  bool SYCLBEllPackMatrix<ValueT>::
  initMatrix(Arccore::MessagePassing::IMessagePassingMng* parallel_mng,
             std::size_t nrows,
             int const* kcol,
             int const* cols)
  {
    alien_debug(
        [&] { cout() << "INIT SYCL MATTRIX"; });
    std::size_t block_nrows = ProfileInternal1024::nbBlocks(nrows) ;

    ProfileInternal1024::computeBlockRowOffset(m_block_row_offset,nrows,kcol) ;

    alien_debug([&] {
                      cout() << "NROWS  = "<<nrows;
                      cout() << "NNZ    = "<<kcol[nrows];
                      cout() << "BNROWS = "<<block_nrows;
                      cout() << "BNNZ   = "<<m_block_row_offset[block_nrows];
                    });
    //delete m_profile1024 ;
    m_block_size = 1024 ;
    m_profile1024 = new ProfileInternal1024{nrows,kcol,cols,m_block_row_offset.data()} ;

    //delete m_matrix1024 ;
    //m_block_values.resize(m_block_row_offset[block_nrows]*m_block_size) ;
    m_matrix1024 = new MatrixInternal1024{m_profile1024};

    return true;
  }

  template <typename ValueT>
  bool SYCLBEllPackMatrix<ValueT>::setMatrixValues(Arccore::Real const* values)
  {
    return m_matrix1024->setMatrixValues(values);
  }

  template <typename ValueT>
  void SYCLBEllPackMatrix<ValueT>::mult(SYCLVector<ValueT> const& x, SYCLVector<ValueT>& y) const
  {
    return m_matrix1024->mult(x.internal()->values(),y.internal()->values());
  }


  template <typename ValueT>
  void SYCLBEllPackMatrix<ValueT>::computeInvDiag(SYCLVector<ValueType>& y) const
  {
    return m_matrix1024->computeInvDiag(y.internal()->values());
  }

/*---------------------------------------------------------------------------*/

template class ALIEN_EXPORT SYCLBEllPackMatrix<double>;
/*---------------------------------------------------------------------------*/

} // namespace Alien

/*---------------------------------------------------------------------------*/
