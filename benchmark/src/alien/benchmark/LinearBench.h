/*
 * Copyright 2022 IFPEN-CEA
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
 *  SPDX-License-Identifier: Apache-2.0
 */

//
// Created by chevalierc on 23/02/22.
//

#ifndef ALIEN_LINEARBENCH_H
#define ALIEN_LINEARBENCH_H

#include <alien/benchmark/export.h>
#include <alien/benchmark/ILinearProblem.h>
#include <alien/expression/solver/ILinearSolver.h>

namespace Alien::Benchmark
{

ALIEN_BENCHMARK_EXPORT class LinearBench
{
 public:
  explicit LinearBench(std::unique_ptr<ILinearProblem>&& lp)
  : m_lp(std::move(lp))
  {
  }

  ~LinearBench() = default;

  ALIEN_BENCHMARK_EXPORT void solve(ILinearSolver* solver) const;

 private:
  std::unique_ptr<ILinearProblem> m_lp;
};

} // namespace Alien::Benchmark

#endif //ALIEN_LINEARBENCH_H
