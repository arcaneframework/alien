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

#include <alien/core/backend/BackEnd.h>
#include <alien/core/backend/LinearSolver.h>

namespace Alien {

class Matrix;

class Vector;

class Options;

class ILinearSolver;

extern std::unique_ptr<IInternalLinearSolver> LinearSolverFactory(const BackEnd::Options& options);

extern std::unique_ptr<IInternalLinearSolver> LinearSolverFactory();
} // namespace Alien


namespace Alien
{
namespace BackEnd
{
  namespace tag
  {
    struct hypre
    {
    };
  } // namespace tag
} // namespace BackEnd


class Plugin : public BackEnd::IPlugin
{
public:
  // factories to default solver
  std::unique_ptr<IInternalLinearSolver> solver_factory()
  {
    return LinearSolverFactory();
  }

  // factories to build solver
  std::unique_ptr<IInternalLinearSolver> solver_factory(const Alien::BackEnd::Options& options)
  {
    return LinearSolverFactory(options);
  }

  void registerMatrixConverters(std::map<std::pair<BackEndId, BackEndId>, IMatrixConverter*>& converters);
  void registerVectorConverters(std::map<std::pair<BackEndId, BackEndId>, IVectorConverter*>& converters);
  void registerMatrixFactory(std::map<BackEndId, BackEnd::MatrixFactory>& matrixFactories);
  void registerVectorFactory(std::map<BackEndId, BackEnd::VectorFactory>& vectorFactories);

  void init();

  BackEndId name() { return "hypre"; }
};

extern "C" {
  ALIEN_EXPORT Plugin* create();
  ALIEN_EXPORT void destroy(Plugin* p);
}

} // namespace Alien
