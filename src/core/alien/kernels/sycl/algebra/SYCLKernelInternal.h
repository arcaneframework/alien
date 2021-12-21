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

/*---------------------------------------------------------------------------*/

namespace Alien::SYCLInternal
{

/*---------------------------------------------------------------------------*/

class KernelInternal
{
private :
  int m_dot_algo = 0 ;

public:
  KernelInternal()
  {
    m_env = SYCLEnv::instance() ;

    m_max_num_groups      = m_env->maxNumGroups() ;
    m_max_work_group_size = m_env->maxWorkGroupSize() ;
    m_total_threads = m_env->maxNumThreads() ;
  }

  virtual ~KernelInternal() {}


  void setDotAlgo(int dot_algo)
  {
    m_dot_algo = dot_algo ;
  }

  template <typename T>
  void assign(T const a,
             cl::sycl::buffer<T>& y)
  {
    cl::sycl::range<1> work_items{m_total_threads};
    {
        m_env->internal()->queue().submit([&](cl::sycl::handler& cgh)
                 {
                   auto access_y = y.template get_access<cl::sycl::access::mode::read_write>(cgh);

                   cgh.parallel_for<class vector_axpy>(cl::sycl::range<1>{m_total_threads}, [=] (cl::sycl::item<1> itemId)
                                                      {
                                                         auto id = itemId.get_id(0);
                                                         for (auto i = id; i < y.get_count(); i += itemId.get_range()[0])
                                                            access_y[i] = a;
                                                      });
                 });
    }
  }

  template <typename T, typename Lambda>
  void apply(Lambda const& lambda,
             cl::sycl::buffer<T>& y)
  {
    cl::sycl::range<1> work_items{m_total_threads};
    {
        m_env->internal()->queue().submit([&](cl::sycl::handler& cgh)
                 {
                   auto access_y = y.template get_access<cl::sycl::access::mode::read_write>(cgh);

                   cgh.parallel_for<class vector_apply>(cl::sycl::range<1>{m_total_threads}, [=] (cl::sycl::item<1> itemId)
                                                      {
                                                         auto id = itemId.get_id(0);
                                                         for (auto i = id; i < y.get_count(); i += itemId.get_range()[0])
                                                            access_y[i] = lambda(i);
                                                      });
                 });
    }
  }

  template <typename T>
  void scal(T a,
             cl::sycl::buffer<T>& y)
  {
    cl::sycl::range<1> work_items{m_total_threads};
    {
        m_env->internal()->queue().submit([&](cl::sycl::handler& cgh)
                 {
                   auto access_y = y.template get_access<cl::sycl::access::mode::read_write>(cgh);

                   cgh.parallel_for<class vector_axpy>(cl::sycl::range<1>{m_total_threads}, [=] (cl::sycl::item<1> itemId)
                                                      {
                                                         auto id = itemId.get_id(0);
                                                         for (auto i = id; i < y.get_count(); i += itemId.get_range()[0])
                                                            access_y[i] = a*access_y[i];
                                                      });
                 });
    }
  }

  template <typename T>
  void axpy(T const a,
            cl::sycl::buffer<T>& x,
            cl::sycl::buffer<T>& y)
  {
    cl::sycl::range<1> work_items{m_total_threads};
    {
        m_env->internal()->queue().submit([&](cl::sycl::handler& cgh)
                 {
                   auto access_x = x.template get_access<cl::sycl::access::mode::read>(cgh);
                   auto access_y = y.template get_access<cl::sycl::access::mode::read_write>(cgh);

                   cgh.parallel_for<class vector_axpy>(cl::sycl::range<1>{m_total_threads}, [=] (cl::sycl::item<1> itemId)
                                                      {
                                                         auto id = itemId.get_id(0);
                                                         for (auto i = id; i < y.get_count(); i += itemId.get_range()[0])
                                                            access_y[i] += a * access_x[i];
                                                      });
                 });
    }
  }

  template <typename T>
  void pointwiseMult(cl::sycl::buffer<T>& x,
                     cl::sycl::buffer<T>& y,
                     cl::sycl::buffer<T>& z)
  {
    cl::sycl::range<1> work_items{m_total_threads};
    {
        m_env->internal()->queue().submit([&](cl::sycl::handler& cgh)
                 {
                   auto access_x = x.template get_access<cl::sycl::access::mode::read>(cgh);
                   auto access_y = y.template get_access<cl::sycl::access::mode::read>(cgh);
                   auto access_z = z.template get_access<cl::sycl::access::mode::read_write>(cgh);

                   cgh.parallel_for<class vector_axpy>(cl::sycl::range<1>{m_total_threads}, [=] (cl::sycl::item<1> itemId)
                                                      {
                                                         auto id = itemId.get_id(0);
                                                         for (auto i = id; i < y.get_count(); i += itemId.get_range()[0])
                                                            access_z[i] = access_x[i] * access_y[i];
                                                      });
                 });
    }
  }


  template <typename T>
  void copy(cl::sycl::buffer<T>& x,
            cl::sycl::buffer<T>& y)
  {
    cl::sycl::range<1> work_items{m_total_threads};
    {
        m_env->internal()->queue().submit([&](cl::sycl::handler& cgh)
                 {
                   auto access_x = x.template get_access<cl::sycl::access::mode::read>(cgh);
                   auto access_y = y.template get_access<cl::sycl::access::mode::read_write>(cgh);

                   cgh.parallel_for<class vector_axpy>(cl::sycl::range<1>{m_total_threads}, [=] (cl::sycl::item<1> itemId)
                                                      {
                                                         auto id = itemId.get_id(0);
                                                         for (auto i = id; i < y.get_count(); i += itemId.get_range()[0])
                                                            access_y[i] = access_x[i];
                                                      });
                 });
    }
  }


  template <typename T>
  class sycl_reduction;


  // to make global size multiple of local size
  template <typename index_t>
  inline index_t round_up(const index_t x, const index_t y) {
    return ((x + y - 1) / y) * y;
  }


  template <typename T>
  class sycl_reduction_sum;

  template <typename T>
  T reduce_sum(cl::sycl::buffer<T>& x,
               cl::sycl::buffer<T>& y)
  {

    auto& w = getWorkBuffer<T>(x.get_count()) ;

    m_env->internal()->queue().submit([&](cl::sycl::handler& cgh)
             {
               auto access_x = x.template get_access<cl::sycl::access::mode::read>(cgh);
               auto access_y = y.template get_access<cl::sycl::access::mode::read>(cgh);
               //auto access_w = w.template get_access<cl::sycl::access::mode::write>(cgh);
               auto access_w = cl::sycl::accessor { w, cgh, cl::sycl::write_only, cl::sycl::property::no_init{}};
               //auto access_w = w.get_access(cgh,cl::sycl::write_only, cl::sycl::property::no_init{});
               cgh.parallel_for<class vector_dot>(cl::sycl::range<1>{m_total_threads}, [=] (cl::sycl::item<1> itemId)
                                                  {
                                                     auto id = itemId.get_id(0);
                                                     for (auto i = id; i < y.get_count(); i += itemId.get_range()[0])
                                                        access_w[i] = access_x[i]*access_y[i];
                                                  });
             });

    std::size_t local = m_max_work_group_size;
    std::size_t length = x.get_count();


    int level = 0 ;
    {
        /* Each iteration of the do loop applies one level of reduction until
         * the input is of length 1 (i.e. the reduction is complete). */
        do
        {
          auto round_length = round_up(length,local) ;
          auto f = [length, round_length, local, &w](cl::sycl::handler& h) mutable
                   {
                      cl::sycl::nd_range<1> range{cl::sycl::range<1>{round_length},
                                                  cl::sycl::range<1>{local}};
                      auto access_w = w.template get_access<cl::sycl::access::mode::read_write>(h);

                      cl::sycl::accessor<T, 1, cl::sycl::access::mode::read_write,cl::sycl::access::target::local>
                        scratch(cl::sycl::range<1>(local), h);

                      /* The parallel_for invocation chosen is the variant with an nd_item
                       * parameter, since the code requires barriers for correctness. */
                      h.parallel_for<sycl_reduction_sum<T>>(range, [access_w, scratch, local, length](cl::sycl::nd_item<1> id)
                                                           {
                                                             std::size_t globalid = id.get_global_id(0);
                                                             std::size_t localid = id.get_local_id(0);

                                                            /* All threads collectively read from global memory into local.
                                                             * The barrier ensures all threads' IO is resolved before
                                                             * execution continues (strictly speaking, all threads within
                                                             * a single work-group - there is no co-ordination between
                                                             * work-groups, only work-items). */
                                                             scratch[localid] = (globalid < length)?  access_w[globalid] : 0. ;
                                                             id.barrier(cl::sycl::access::fence_space::local_space);

                                                            /* Apply the reduction operation between the current local
                                                             * id and the one on the other half of the vector. */
                                                            if (globalid < length)
                                                            {
                                                              //int min = (length < local) ? length : local;
                                                              int min = local ;
                                                              for (std::size_t offset = min / 2; offset > 0; offset /= 2)
                                                              //for (std::size_t offset = id.get_local_range(0) / 2; offset > 0; offset /= 2)
                                                              {
                                                                if (localid < offset)
                                                                {
                                                                   scratch[localid] += scratch[localid + offset];
                                                                }
                                                                id.barrier(cl::sycl::access::fence_space::local_space);
                                                              }
                                                              /* The final result will be stored in local id 0. */
                                                              if (localid == 0)
                                                              {
                                                                access_w[id.get_group(0)] = scratch[localid];
                                                              }
                                                            }
                                                      });
                 };
          m_env->internal()->queue().submit(f);
          /* At this point, you could queue::wait_and_throw() to ensure that
           * errors are caught quickly. However, this would likely impact
           * performance negatively. */
          length = (length+local-1) / local;
          ++level ;
        } while (length > 1);
    }
    auto h_w = w.template get_access<cl::sycl::access::mode::read>();
    T sum = h_w[0] ;
    return sum;
  }


  template <typename T>
  class sycl_map_reduction_sum0;

  template <typename T>
  class sycl_map_reduction_sum;

  template <typename T>
  T map_reduce_sum(cl::sycl::buffer<T>& x,
                   cl::sycl::buffer<T>& y)
  {
    auto& w = getWorkBuffer<T>(x.get_count()) ;

   std::size_t local = m_max_work_group_size;
   std::size_t length = x.get_count();

    int level = 0 ;
    {
        /* Each iteration of the do loop applies one level of reduction until
         * the input is of length 1 (i.e. the reduction is complete). */
        //do
        {
          auto round_length = round_up(length,local) ;
          auto f0 = [length, round_length, local, &x,&y, &w](cl::sycl::handler& h) mutable
                   {
                      cl::sycl::nd_range<1> range{cl::sycl::range<1>{round_length},
                                                  cl::sycl::range<1>{local}};
                      auto access_x = x.template get_access<cl::sycl::access::mode::read>(h);
                      auto access_y = y.template get_access<cl::sycl::access::mode::read>(h);
                      //auto access_w = w.template get_access<cl::sycl::access::mode::read_write>(h);
                      auto access_w = cl::sycl::accessor { w, h, cl::sycl::read_write, cl::sycl::property::no_init{}};

                      cl::sycl::accessor<T, 1, cl::sycl::access::mode::read_write,cl::sycl::access::target::local>
                        scratch(cl::sycl::range<1>(local), h);

                      /* The parallel_for invocation chosen is the variant with an nd_item
                       * parameter, since the code requires barriers for correctness. */
                      h.parallel_for<sycl_map_reduction_sum0<T>>(range,
                                                                 [access_x,access_y,access_w, scratch, local, length](cl::sycl::nd_item<1> id)
                                                                 {
                                                                   std::size_t globalid = id.get_global_id(0);
                                                                   std::size_t localid = id.get_local_id(0);

                                                                  /* All threads collectively read from global memory into local.
                                                                   * The barrier ensures all threads' IO is resolved before
                                                                   * execution continues (strictly speaking, all threads within
                                                                   * a single work-group - there is no co-ordination between
                                                                   * work-groups, only work-items). */
                                                                   scratch[localid] = (globalid < length)?  access_x[globalid]*access_y[globalid] : 0. ;

                                                                   id.barrier(cl::sycl::access::fence_space::local_space);

                                                                  /* Apply the reduction operation between the current local
                                                                   * id and the one on the other half of the vector. */
                                                                  if (globalid < length)
                                                                  {
                                                                    //int min = (length < local) ? length : local;
                                                                    int min = local ;
                                                                    for (std::size_t offset = min / 2; offset > 0; offset /= 2)
                                                                    //for (std::size_t offset = id.get_local_range(0) / 2; offset > 0; offset /= 2)
                                                                    {
                                                                      if (localid < offset)
                                                                      {
                                                                         scratch[localid] += scratch[localid + offset];
                                                                      }
                                                                      id.barrier(cl::sycl::access::fence_space::local_space);
                                                                    }
                                                                    /* The final result will be stored in local id 0. */
                                                                    if (localid == 0)
                                                                    {
                                                                      access_w[id.get_group(0)] = scratch[localid];
                                                                    }
                                                                  }
                                                                });
                 };

          auto f1 = [length, round_length, local, &w](cl::sycl::handler& h) mutable
                   {
                      cl::sycl::nd_range<1> range{cl::sycl::range<1>{round_length},
                                                  cl::sycl::range<1>{local}};
                      auto access_w = w.template get_access<cl::sycl::access::mode::read_write>(h);

                      cl::sycl::accessor<T, 1, cl::sycl::access::mode::read_write,cl::sycl::access::target::local>
                        scratch(cl::sycl::range<1>(local), h);

                      /* The parallel_for invocation chosen is the variant with an nd_item
                       * parameter, since the code requires barriers for correctness. */
                      h.parallel_for<sycl_map_reduction_sum<T>>(range,
                                                                [access_w, scratch, local, length](cl::sycl::nd_item<1> id)
                                                                {
                                                                   std::size_t globalid = id.get_global_id(0);
                                                                   std::size_t localid = id.get_local_id(0);

                                                                  /* All threads collectively read from global memory into local.
                                                                   * The barrier ensures all threads' IO is resolved before
                                                                   * execution continues (strictly speaking, all threads within
                                                                   * a single work-group - there is no co-ordination between
                                                                   * work-groups, only work-items). */
                                                                   scratch[localid] = (globalid < length)?  access_w[globalid] : 0. ;
                                                                   id.barrier(cl::sycl::access::fence_space::local_space);

                                                                  /* Apply the reduction operation between the current local
                                                                   * id and the one on the other half of the vector. */
                                                                  if (globalid < length)
                                                                  {
                                                                    //int min = (length < local) ? length : local;
                                                                    int min = local ;
                                                                    for (std::size_t offset = min / 2; offset > 0; offset /= 2)
                                                                    //for (std::size_t offset = id.get_local_range(0) / 2; offset > 0; offset /= 2)
                                                                    {
                                                                      if (localid < offset)
                                                                      {
                                                                         scratch[localid] += scratch[localid + offset];
                                                                      }
                                                                      id.barrier(cl::sycl::access::fence_space::local_space);
                                                                    }
                                                                    /* The final result will be stored in local id 0. */
                                                                    if (localid == 0)
                                                                    {
                                                                      access_w[id.get_group(0)] = scratch[localid];
                                                                    }
                                                                  }
                                                                });
                 };
          if(level==0)
            m_env->internal()->queue().submit(f0);
          else
            m_env->internal()->queue().submit(f1);
          /* At this point, you could queue::wait_and_throw() to ensure that
           * errors are caught quickly. However, this would likely impact
           * performance negatively. */
          length = (length+local-1) / local;
          ++level ;
        } //while (length > 1);
    }
    auto h_x = w.template get_access<cl::sycl::access::mode::read>();
    //T sum = h_x[0] ;
    T sum = 0 ;
    for(int i=0;i<length;++i)
      sum += h_x[i] ;
    return sum;
  }

  template <typename T>
  class sycl_map2_reduction_sum0;

  template <typename T>
  T map2_reduce_sum(cl::sycl::buffer<T>& x,
                    cl::sycl::buffer<T>& y)
  {
   std::size_t local = m_max_work_group_size;
   std::size_t length = x.get_count();

   T sum_init = 0 ;
   cl::sycl::buffer<T> sum{&sum_init, 1};

    {
        /* Each iteration of the do loop applies one level of reduction until
         * the input is of length 1 (i.e. the reduction is complete). */
        //do
        {
          auto round_length = round_up(length,local) ;
          auto f0 = [length, round_length, local, &x, &y, &sum](cl::sycl::handler& h) mutable
                   {
                      cl::sycl::nd_range<1> range{cl::sycl::range<1>{round_length},
                                                  cl::sycl::range<1>{local}};
                      auto access_x = x.template get_access<cl::sycl::access::mode::read>(h);
                      auto access_y = y.template get_access<cl::sycl::access::mode::read>(h);

                      cl::sycl::accessor access_sum {sum, h};

                      auto sumReduction = cl::sycl::reduction(access_sum, cl::sycl::plus<T>());

                      cl::sycl::accessor<T, 1, cl::sycl::access::mode::read_write,cl::sycl::access::target::local>
                        scratch(cl::sycl::range<1>(local), h);

                      /* The parallel_for invocation chosen is the variant with an nd_item
                       * parameter, since the code requires barriers for correctness. */
                      h.parallel_for<sycl_map2_reduction_sum0<T>>(range,
                                                                  sumReduction,
                                                                  [access_x,access_y, scratch, local, length](cl::sycl::nd_item<1> id, auto &sum)
                                                                  {
                                                                     std::size_t globalid = id.get_global_id(0);
                                                                     std::size_t localid = id.get_local_id(0);

                                                                    /* All threads collectively read from global memory into local.
                                                                     * The barrier ensures all threads' IO is resolved before
                                                                     * execution continues (strictly speaking, all threads within
                                                                     * a single work-group - there is no co-ordination between
                                                                     * work-groups, only work-items). */
                                                                     scratch[localid] = (globalid < length)?  access_x[globalid]*access_y[globalid] : 0. ;

                                                                     id.barrier(cl::sycl::access::fence_space::local_space);

                                                                    /* Apply the reduction operation between the current local
                                                                     * id and the one on the other half of the vector. */
                                                                    if (globalid < length)
                                                                    {
                                                                      //int min = (length < local) ? length : local;
                                                                      int min = local ;
                                                                      for (std::size_t offset = min / 2; offset > 0; offset /= 2)
                                                                      //for (std::size_t offset = id.get_local_range(0) / 2; offset > 0; offset /= 2)
                                                                      {
                                                                        if (localid < offset)
                                                                        {
                                                                           scratch[localid] += scratch[localid + offset];
                                                                        }
                                                                        id.barrier(cl::sycl::access::fence_space::local_space);
                                                                      }
                                                                      /* The final result will be stored in local id 0. */
                                                                      if (localid == 0)
                                                                      {
                                                                        //access_w[id.get_group(0)] = scratch[localid];
                                                                        sum += scratch[localid];
                                                                      }
                                                                    }
                                                                });
                 };
          m_env->internal()->queue().submit(f0);

          /* At this point, you could queue::wait_and_throw() to ensure that
           * errors are caught quickly. However, this would likely impact
           * performance negatively. */
          length = (length+local-1) / local;
          //++level ;
        } //while (length > 1);
    }
    auto h_sum = sum.template get_access<cl::sycl::access::mode::read>();
    return  h_sum[0];
  }


  template <typename T>
  class sycl_map3_reduction_sum0;

  template <typename T>
  T map3_reduce_sum(cl::sycl::buffer<T>& x,
                    cl::sycl::buffer<T>& y)
  {
   std::size_t local = m_max_work_group_size;
   std::size_t total_threads = m_total_threads ;
   std::size_t length = x.get_count();

   //std::vector<T> group_sum(m_max_num_groups) ;
   cl::sycl::buffer<T> group_sum{m_max_num_groups};

   T sum_init = 0 ;
   cl::sycl::buffer<T> sum{&sum_init,1};

    {
        /* Each iteration of the do loop applies one level of reduction until
         * the input is of length 1 (i.e. the reduction is complete). */
        //do
        {
          auto f0 = [total_threads, length, local, &x, &y,&sum, &group_sum](cl::sycl::handler& h) mutable
                   {
                      cl::sycl::nd_range<1> range{cl::sycl::range<1>{total_threads},
                                                  cl::sycl::range<1>{local}};
                      auto access_x = x.template get_access<cl::sycl::access::mode::read>(h);
                      auto access_y = y.template get_access<cl::sycl::access::mode::read>(h);

                      //auto access_sum = cl::sycl::accessor { group_sum, h, cl::sycl::read_write, cl::sycl::property::no_init{}};

                      cl::sycl::accessor access_sum {sum, h};
                      auto sumReduction = cl::sycl::reduction(access_sum, cl::sycl::plus<T>());

                      cl::sycl::accessor<T, 1, cl::sycl::access::mode::read_write,cl::sycl::access::target::local>
                        scratch(cl::sycl::range<1>(local), h);

                      /* The parallel_for invocation chosen is the variant with an nd_item
                       * parameter, since the code requires barriers for correctness. */

                      h.parallel_for<sycl_map3_reduction_sum0<T>>(range,
                                                                  sumReduction,
                                                                  [access_x,access_y,scratch,local,length,total_threads] (cl::sycl::nd_item<1> id, auto& sum)
                                                                  {
                                                                      std::size_t globalid = id.get_global_id(0);
                                                                      std::size_t localid = id.get_local_id(0);

                                                                      scratch[localid] = (globalid < length)?  access_x[globalid]*access_y[globalid] : 0. ;

                                                                      for (auto i = globalid+total_threads; i < length; i += total_threads)
                                                                        scratch[localid] += access_x[i]*access_y[i];

                                                                      id.barrier(cl::sycl::access::fence_space::local_space);

                                                                      /* Apply the reduction operation between the current local
                                                                       * id and the one on the other half of the vector. */
                                                                      if (globalid < length)
                                                                      {
                                                                        //int min = (length < local) ? length : local;
                                                                        int min = local ;
                                                                        for (std::size_t offset = min / 2; offset > 0; offset /= 2)
                                                                        //for (std::size_t offset = id.get_local_range(0) / 2; offset > 0; offset /= 2)
                                                                        {
                                                                          if (localid < offset)
                                                                          {
                                                                             scratch[localid] += scratch[localid + offset];
                                                                          }
                                                                          id.barrier(cl::sycl::access::fence_space::local_space);
                                                                        }
                                                                      }
                                                                      /* The final result will be stored in local id 0. */
                                                                      if (localid == 0)
                                                                      {
                                                                        //access_sum[id.get_group(0)] = scratch[localid];
                                                                        sum += scratch[localid] ;
                                                                      }

                                                                  });

                 };
          m_env->internal()->queue().submit(f0);

          /* At this point, you could queue::wait_and_throw() to ensure that
           * errors are caught quickly. However, this would likely impact
           * performance negatively. */
          length = (length+local-1) / local;
          //++level ;
        } //while (length > 1);
    }

    return sum.get_host_access()[0];
    /*
    auto h_sum = group_sum.template get_access<cl::sycl::access::mode::read>();
    T sum = 0 ;
    for(int i=0;i<length;++i)
      sum += h_sum[i] ;
    return sum;*/
  }


  template <typename T>
  T reduce_sum2(const std::vector<T>& x)
  {
    T value = 0 ;
    //for( auto v : x)
    //  value += v ;
    //return value ;

    {
      auto num_groups = m_env->internal()->queue().get_device().get_info<cl::sycl::info::device::max_compute_units>();
      // getting the maximum work group size per thread
      auto work_group_size = m_env->internal()->queue().get_device().get_info<cl::sycl::info::device::max_work_group_size>();
      // building the best number of global thread
      auto total_threads = num_groups * work_group_size;

      std::cout<<"LOCAL     ="<<work_group_size<<std::endl ;
      std::cout<<"NB GROUPS ="<<num_groups<<std::endl ;
      std::cout<<"NB THREADS="<<total_threads<<std::endl ;

      /* The buffer is used to initialise the data on the device, but we don't
       * want to copy back and trash it. buffer::set_final_data() tells the
       * SYCL runtime where to put the data when the buffer is destroyed; nullptr
       * indicates not to copy back. The vector's length is used as the global
       * work size (unless that is too large). */
      auto device = m_env->internal()->queue().get_device();
      //std::size_t local = std::min(x.size(),device.get_info<cl::sycl::info::device::max_work_group_size>());
      std::size_t local = device.get_info<cl::sycl::info::device::max_work_group_size>();

      std::size_t length = x.size();

      cl::sycl::buffer<T, 1> xbuf(x.data(), cl::sycl::range<1>(x.size()));
      xbuf.set_final_data(nullptr);

      int level = 0 ;
      {
        /* Each iteration of the do loop applies one level of reduction until
         * the input is of length 1 (i.e. the reduction is complete). */
        do
        {

          auto round_length = round_up(length,local) ;
          std::cout<<"LENGTH :"<<level<<" "<<length<<" "<<round_length<<" "<<local<<std::endl ;
          auto f = [length,round_length,local, &xbuf](cl::sycl::handler& h) mutable
                   {
                      //cl::sycl::nd_range<1> r{cl::sycl::range<1>{std::max(length, local)},
                      //                        cl::sycl::range<1>{std::min(length, local)}};
                      cl::sycl::nd_range<1> r{cl::sycl::range<1>{round_length},
                                              cl::sycl::range<1>{local}};
                      /* Two accessors are used: one to the buffer that is being reduced,
                       * and a second to local memory, used to store intermediate data. */
                      auto x_access = xbuf.template get_access<cl::sycl::access::mode::read_write>(h);

                      cl::sycl::accessor<T, 1, cl::sycl::access::mode::read_write,cl::sycl::access::target::local>
                        scratch(cl::sycl::range<1>(local), h);

                      /* The parallel_for invocation chosen is the variant with an nd_item
                       * parameter, since the code requires barriers for correctness. */
                      h.parallel_for<class reduce_sum2>(r, [x_access, scratch, local, length](cl::sycl::nd_item<1> id)
                                                           {
                                                             std::size_t globalid = id.get_global_id(0);
                                                             std::size_t localid = id.get_local_id(0);

                                                            /* All threads collectively read from global memory into local.
                                                             * The barrier ensures all threads' IO is resolved before
                                                             * execution continues (strictly speaking, all threads within
                                                             * a single work-group - there is no co-ordination between
                                                             * work-groups, only work-items). */
                                                             scratch[localid] = (globalid < length)?  x_access[globalid] : 0. ;
                                                             id.barrier(cl::sycl::access::fence_space::local_space);

                                                            /* Apply the reduction operation between the current local
                                                             * id and the one on the other half of the vector. */
                                                            //#ifdef DEBUG
                                                            if (globalid < length)
                                                            {
                                                              //int min = (length < local) ? length : local;
                                                              int min = local ;
                                                              for (std::size_t offset = min / 2; offset > 0; offset /= 2)
                                                              //for (std::size_t offset = id.get_local_range(0) / 2; offset > 0; offset /= 2)
                                                              {
                                                                if (localid < offset)
                                                                {
                                                                   scratch[localid] += scratch[localid + offset];
                                                                }
                                                                id.barrier(cl::sycl::access::fence_space::local_space);
                                                              }
                                                              /* The final result will be stored in local id 0. */
                                                              if (localid == 0)
                                                              {
                                                                x_access[id.get_group(0)] = scratch[localid];
                                                              }
                                                            }
                                                            //#endif
                                                      });
                 };
          m_env->internal()->queue().submit(f);
          /* At this point, you could queue::wait_and_throw() to ensure that
           * errors are caught quickly. However, this would likely impact
           * performance negatively. */
          length = (length+local-1) / local;
          std::cout<<"AFTER LENGTH :"<<level<<" new length"<<length<<" "<<local<<std::endl ;
          ++level ;
        } while (length > 1);
      }
      /* It is always sensible to wrap host accessors in their own scope as
       * kernels using the buffers they access are blocked for the length
       * of the accessor's lifetime. */
      auto hI = xbuf.template get_access<cl::sycl::access::mode::read>();
      value = hI[0];
    }
    //value = x[0] ;
    return value;
  }

  template <typename T>
  T sycl_reduce_sum(cl::sycl::buffer<T>& x,
                    cl::sycl::buffer<T>& y)
  {
    using namespace cl;

    T sum_init = 0 ;
    sycl::buffer<T> sum_buff{&sum_init,1};

    m_env->internal()->queue().submit([&](sycl::handler &cgh)
                                      {
                                        auto access_x = x.template get_access<cl::sycl::access::mode::read>(cgh);
                                        auto access_y = y.template get_access<cl::sycl::access::mode::read>(cgh);
                                        sycl::accessor sum_acc {sum_buff, cgh};
                                        auto sumReduction = sycl::reduction(sum_acc, sycl::plus<T>());
                                        cgh.parallel_for(sycl::range<1>{x.get_count()},
                                                         sumReduction,
                                                         [=](sycl::id<1> idx, auto &sum)
                                                         {
                                                           sum += access_x[idx]*access_y[idx];
                                                         });
                                      });

    return sum_buff.get_host_access()[0];
  }

  template <typename T>
  T dot(cl::sycl::buffer<T>& x,
        cl::sycl::buffer<T>& y)
  {
    std::cout<<"DOT ALGO : "<<m_dot_algo<<std::endl ;
    switch(m_dot_algo)
    {
      case 0 :
        return reduce_sum(x,y) ;
      case 1 :
        return map_reduce_sum(x,y) ;
      case 2 :
        return map2_reduce_sum(x,y) ;
      case 3 :
        return map3_reduce_sum(x,y) ;
      default:
        return sycl_reduce_sum(x,y) ;
    }


    //return sycl_reduce_sum(w) ;
    /*
    using namespace cl;
    T sumResult = 0;

    sycl::buffer<T> sum_buff{&sumResult,1};

    m_env->internal()->queue().submit([&](sycl::handler &cgh)
                                      {
                                        auto x_acc = x.template get_access<sycl::access_mode::read>(cgh);
                                        auto y_acc = y.template get_access<sycl::access_mode::read>(cgh);

                                        sycl::accessor sum_acc {sum_buff, cgh};
                                        auto sumReduction = sycl::reduction(sum_acc, sycl::plus<T>());
                                        cgh.parallel_for(sycl::range<1>{x.get_count()},
                                                         sumReduction,
                                                         [=](sycl::id<1> idx, auto &sum)
                                                         {
                                                           sum += x_acc[idx]*y_acc[idx];
                                                         });
                                      });
    return sum_buff.get_host_access()[0];
    */
  }

 private:
  SYCLEnv*    m_env                 = nullptr ;
  std::size_t m_max_num_groups      = 0 ;
  std::size_t m_max_work_group_size = 0 ;
  std::size_t m_total_threads       = 0 ;

  template<typename T>
  cl::sycl::buffer<T>& getWorkBuffer(std::size_t size) ;

  mutable cl::sycl::buffer<double>* m_double_work = nullptr;

};

template<>
cl::sycl::buffer<double>& KernelInternal::getWorkBuffer(std::size_t size)
{
  if(m_double_work==nullptr)
  {
    m_double_work = new cl::sycl::buffer<double>(size) ;
    m_double_work->set_final_data(nullptr);

  }
  else
  {
    if(size>m_double_work->get_count())
    {
      delete m_double_work ;
      m_double_work = new cl::sycl::buffer<double>(size) ;
      m_double_work->set_final_data(nullptr);
    }
  }
  return *m_double_work ;
}

/*---------------------------------------------------------------------------*/

} // namespace Alien::SYCLInternal

/*---------------------------------------------------------------------------*/
