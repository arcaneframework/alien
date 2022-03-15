/*
* Copyright 2021 IFPEN-CEA
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

#include "hypre_instance.h"

#include <HYPRE_utilities.h>

#ifdef ALIEN_HYPRE_CUDA
#include <cuda_runtime.h>
#endif // ALIEN_HYPRE_CUDA


// Function is not always defined in HYPRE_utilities.h
extern "C" HYPRE_Int HYPRE_Init();

namespace
{
bool hypre_initialized = false;
}

namespace Alien::Hypre
{

void hypre_init_if_needed()
{
  if (hypre_initialized)
    return;

#ifdef ALIEN_HYPRE_CUDA
  cudaSetDevice(0);
#endif // ALIEN_HYPRE_CUDA

  HYPRE_Init();
  hypre_initialized = true;

#ifdef ALIEN_HYPRE_CUDA
  /* AMG in GPU memory (default) */
  HYPRE_SetMemoryLocation(HYPRE_MEMORY_DEVICE);
  /* setup AMG on GPUs */
  HYPRE_SetExecutionPolicy(HYPRE_EXEC_DEVICE);
  /* use hypre's SpGEMM instead of cuSPARSE */
  HYPRE_SetSpGemmUseCusparse(false);
  /* use GPU RNG */
  HYPRE_SetUseGpuRand(true);
  HYPRE_PrintDeviceInfo();
#endif //ALIEN_HYPRE_CUDA
}

} // namespace Alien::Hypre