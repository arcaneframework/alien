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



#include <alien/ginkgo/machine_backend.h>

namespace Alien::Ginkgo
{

  std::string ginkgo_executor::target_machine = "reference";

  std::map<std::string, std::function<std::shared_ptr<gko::Executor>()>>
  ginkgo_executor::exec_map{
    { "omp", [] { return gko::OmpExecutor::create(); } },
    { "cuda",
      [] {
        return gko::CudaExecutor::create(0, gko::OmpExecutor::create(),
                                         true);
      } },
    { "hip",
      [] {
        return gko::HipExecutor::create(0, gko::OmpExecutor::create(),
                                        true);
      } },
    { "dpcpp",
      [] {
        return gko::DpcppExecutor::create(0,
                                          gko::OmpExecutor::create());
      } },
    { "reference", [] { return gko::ReferenceExecutor::create(); } }
  };
} // namespace
