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

#include "../trilinos_vector.h"

#include <alien/core/backend/IVectorConverter.h>
#include <alien/core/backend/VectorConverterRegisterer.h>
#include <alien/kernels/simple_csr/SimpleCSRVector.h>
#include <alien/kernels/simple_csr/SimpleCSRBackEnd.h>

#include <alien/trilinos/backend.h>

class Trilinos_to_SimpleCSR_VectorConverter : public Alien::IVectorConverter
{
 public:
  Trilinos_to_SimpleCSR_VectorConverter() {}

  virtual ~Trilinos_to_SimpleCSR_VectorConverter() {}

 public:
  Alien::BackEndId sourceBackend() const { return Alien::AlgebraTraits<Alien::BackEnd::tag::trilinos>::name(); }

  Alien::BackEndId targetBackend() const { return Alien::AlgebraTraits<Alien::BackEnd::tag::simplecsr>::name(); }

  void convert(const Alien::IVectorImpl* sourceImpl, Alien::IVectorImpl* targetImpl) const;
};

void Trilinos_to_SimpleCSR_VectorConverter::convert(
const Alien::IVectorImpl* sourceImpl, Alien::IVectorImpl* targetImpl) const
{
  const auto& v = cast<Alien::Trilinos::Vector>(sourceImpl, sourceBackend());
  auto& v2 = cast<Alien::SimpleCSRVector<Arccore::Real>>(targetImpl, targetBackend());


  std::cout << "Converting Trilinos::Vector: " << &v << " to Alien::SimpleCSRVector " << &v2;

  // target values (simpleCSR)
  auto values = v2.values();

  // update v (trilinos values)
  v.getValues(values);
}

REGISTER_VECTOR_CONVERTER(Trilinos_to_SimpleCSR_VectorConverter);
