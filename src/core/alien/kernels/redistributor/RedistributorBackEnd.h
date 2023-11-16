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

namespace Arccore::MessagePassing
{
class IMessagePassingMng;
}

/*---------------------------------------------------------------------------*/

namespace Alien
{

/*---------------------------------------------------------------------------*/

class RedistributorLinearSolver;
class RedistributorMatrix;
class RedistributorVector;
class Space;
template <class Matrix, class Vector>
class IInternalLinearAlgebra;
template <class Matrix, class Vector>
class IInternalLinearSolver;

/*---------------------------------------------------------------------------*/

namespace BackEnd
{
  namespace tag
  {
    struct redistributor
    {};
  } // namespace tag
} // namespace BackEnd

template <>
struct AlgebraTraits<BackEnd::tag::redistributor>
{
  using matrix_type = RedistributorMatrix;
  using vector_type = RedistributorVector;

  static BackEndId name() { return "redistributor"; }
};

/*---------------------------------------------------------------------------*/

} // namespace Alien

/*---------------------------------------------------------------------------*/
