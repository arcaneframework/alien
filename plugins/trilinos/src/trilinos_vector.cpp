/*
 * Copyright 2020-2021 IFPEN-CEA
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

#include "trilinos_vector.h"

#include <alien/trilinos/backend.h>

#include <arccore/message_passing_mpi/MpiMessagePassingMng.h>

//#include <HYPRE.h>

namespace Alien::Trilinos
{
Vector::Vector(const MultiVectorImpl* multi_impl)
: IVectorImpl(multi_impl, AlgebraTraits<BackEnd::tag::trilinos>::name())
, vec(nullptr)
{
  std::cout << " -------------------------- >>> VEC ctor !!! " << std::endl;

  // communicator
  using Teuchos::RCP;
  using Teuchos::Comm;
  using Teuchos::MpiComm;
  MPI_Comm yourComm = MPI_COMM_WORLD;
  t_comm = RCP<const Comm<int>>(new MpiComm<int> (yourComm)); // Récupérer le communicateur Arcane ?

 /* auto block_size = 1;
  const auto* block = this->block();
  if (block)
    block_size *= block->size();
  else if (this->vblock())
    throw Arccore::FatalErrorException(A_FUNCINFO, "Not implemented yet");

  const auto localOffset = distribution().offset();
  const auto localSize = distribution().localSize();
  const auto ilower = localOffset * block_size;
  const auto iupper = ilower + localSize * block_size - 1;

  auto* pm = dynamic_cast<Arccore::MessagePassing::Mpi::MpiMessagePassingMng*>(distribution().parallelMng());
  m_comm = pm ? (*pm->getMPIComm()) : MPI_COMM_WORLD;

  setProfile(ilower, iupper);*/
}

Vector::~Vector()
{
  if(vec)
    vec.release();
}

void Vector::setProfile(int ilower, int iupper, int numGlobalElts, int numLocalElts)
{

  using Teuchos::RCP;
  using Teuchos::rcp;
  typedef Kokkos::Compat::KokkosOpenMPWrapperNode Node;
  typedef typename Tpetra::Map<>::local_ordinal_type      LO;
  typedef typename Tpetra::Map<>::global_ordinal_type     GO;
  typedef Tpetra::Map<LO,GO,Node>                         map_type;

  typedef Tpetra::Vector<SC, LO, GO, Node>                vec_type;

  //if already exists, dealloc
  if (vec)
    vec.release();

  // map
  RCP<const map_type> map = rcp (new map_type (numGlobalElts,numLocalElts,0,t_comm));
  vec = std::make_unique<Teuchos::RCP<vec_type>> (new vec_type (map, numLocalElts));

}

void Vector::setValues(Arccore::ConstArrayView<double> values)
{
  auto ncols = values.size();
  for(size_t i = 0; i < ncols; i++) {
    (*vec)->replaceLocalValue(i,values[i]);
  }
}

void Vector::getValues(Arccore::ArrayView<double> values) const
{
  auto data = (*vec)->getDataNonConst();
  auto ncols = values.size();
  for(size_t i = 0; i < ncols; i++) {
    values[i]=data[i];
  }
}

void Vector::assemble()
{
  // not needed ?
}

} // namespace Alien::Hypre