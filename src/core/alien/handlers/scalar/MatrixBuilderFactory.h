/*
 * Copyright 2023 IFPEN-CEA
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
// Created by chevalierc on 22/05/23.
//

#ifndef ALIEN_MATRIXBUILDERFACTORY_H
#define ALIEN_MATRIXBUILDERFACTORY_H

#include <memory>
#include <functional>

#include <alien/handlers/scalar/IDirectMatrixBuilder.h>
#include <alien/core/backend/BackEnd.h>

template <>
struct std::hash<Alien::BackEndId>
{
  std::size_t operator()(Alien::BackEndId const& backend) const noexcept
  {
    return std::hash<std::string>{}(backend.localstr());
  }
};

namespace Alien
{
namespace Common

{
  class ALIEN_EXPORT MatrixBuilderFactory
  {
   public:
    using BackEndId = Alien::BackEndId;
    using Factory = std::function<std::unique_ptr<Alien::Common::IDirectMatrixBuilder>(IMatrix&, DirectMatrixOptions::ResetFlag,
                                                                                       DirectMatrixOptions::SymmetricFlag)>;

    MatrixBuilderFactory(BackEndId backend, Factory f);

    static std::optional<Factory> get(BackEndId backend);

   private:
    static inline std::unordered_map<Alien::BackEndId, Factory> m_db = {};
  };
} // namespace Common
} // namespace Alien

#endif //ALIEN_MATRIXBUILDERFACTORY_H
