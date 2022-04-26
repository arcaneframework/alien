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

class SimpleCSR_to_Trilinos_VectorConverter : public Alien::IVectorConverter
{
 public:
  SimpleCSR_to_Trilinos_VectorConverter() {}

  virtual ~SimpleCSR_to_Trilinos_VectorConverter() {}

 public:
  Alien::BackEndId sourceBackend() const { return Alien::AlgebraTraits<Alien::BackEnd::tag::simplecsr>::name(); }

  Alien::BackEndId targetBackend() const { return Alien::AlgebraTraits<Alien::BackEnd::tag::trilinos>::name(); }

  void convert(const Alien::IVectorImpl* sourceImpl, Alien::IVectorImpl* targetImpl) const;
};

void SimpleCSR_to_Trilinos_VectorConverter::convert(const Alien::IVectorImpl* sourceImpl,
                                                 Alien::IVectorImpl* targetImpl) const
{
  const auto& v = cast<Alien::SimpleCSRVector<Arccore::Real>>(sourceImpl, sourceBackend());
  auto& v2 = cast<Alien::Trilinos::Vector>(targetImpl, targetBackend());

  std:: cout << "Converting Alien::SimpleCSRVector: " << &v << " to Trilinos::Vector " << &v2 << std::endl;



  auto block_size = 1;
  const auto* block = v2.block();
  if (v2.block())
    block_size *= block->size();
  else if (v2.vblock())
    throw Arccore::FatalErrorException(A_FUNCINFO, "Not implemented yet");

  std:: cout << "block_size : " << block_size << std::endl;

  // destination
  const auto localOffset = v2.distribution().offset();
  const auto localSize = v2.distribution().localSize();
  const auto ilower = localOffset * block_size;
  const auto iupper = ilower + localSize * block_size - 1;

  // source
  const auto& dist = sourceImpl->distribution();
  const int globalSize = dist.globalSize();  //dist.rowSpace().size() ;


  std:: cout  << "Vector range : "
           << "[" << ilower << ":" << iupper << "]"
           << "LocalSize : " << localSize
           << "GlobalSize : " << globalSize
           << std::endl;

  v2.setProfile(ilower, iupper, localSize, globalSize);

  /*auto values = v.values();

  v2.setValues(values);

//  v2.assemble();*/
}

REGISTER_VECTOR_CONVERTER(SimpleCSR_to_Trilinos_VectorConverter);
