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
  std::cout << " -------------------------- >>> VEC ctor : " << vec << std::endl;

  // communicator
  using Teuchos::RCP;
  using Teuchos::Comm;
  using Teuchos::MpiComm;
  MPI_Comm yourComm = MPI_COMM_WORLD;
  t_comm = RCP<const Comm<int>>(new MpiComm<int> (yourComm)); // Récupérer le communicateur Arcane ?

  // allocate by calling setProfile
  auto block_size = 1;
  const auto* block = this->block();
  if (block)
    block_size *= block->size();
  else if (this->vblock())
    throw Arccore::FatalErrorException(A_FUNCINFO, "Not implemented yet");

  const auto localOffset = distribution().offset();
  const auto localSize = distribution().localSize();
  const auto ilower = localOffset * block_size;
  const auto iupper = ilower + localSize * block_size - 1;
  const int globalSize = distribution().globalSize();

  /*std:: cout  << "Vector range : "
            << "[" << ilower << ":" << iupper << "]"
            << "LocalSize : " << localSize
            << "GlobalSize : " << globalSize
            << std::endl;*/

  setProfile(ilower, iupper, globalSize, localSize);
}

Vector::~Vector()
{
  if(vec)
    //vec.release();
    delete vec;
}

void Vector::setProfile(int ilower, int iupper, int numGlobalElts, int numLocalElts)
{
  std::cout << " -------------------------- >>> enter VEC set profile : " << vec << std::endl;

  using Teuchos::RCP;
  using Teuchos::rcp;
  typedef Kokkos::Compat::KokkosOpenMPWrapperNode Node;
  typedef typename Tpetra::Map<>::local_ordinal_type      LO;
  typedef typename Tpetra::Map<>::global_ordinal_type     GO;
  typedef Tpetra::Map<LO,GO,Node>                         map_type;

  typedef Tpetra::Vector<SC, LO, GO, Node>                vec_type;

  //if already exists, dealloc
  if (vec)
    //vec.release();
    delete vec;

  // map
  RCP<const map_type> map = rcp (new map_type (numGlobalElts, numLocalElts,0,t_comm));
 // vec = std::make_unique<Teuchos::RCP<vec_type>> (new vec_type (map, numLocalElts));
  vec = new Teuchos::RCP<vec_type>(new vec_type (map, numLocalElts));

  std::cout << " -------------------------- >>> exit VEC set profile : " << vec << std::endl;

}

void Vector::setValues(Arccore::ConstArrayView<double> values)
{

  std::cout << std::endl << " -------------------------- >>> enter VEC set Values " << vec << std::endl;
  auto ncols = values.size();

  /*auto dataB = (*vec)->getDataNonConst();
  for(size_t i = 0; i < ncols; i++) {
   // std::cout << "[" << i << "] Before Read value : " << dataB[i] << std::endl;
  }*/


  // Locally, with Tpetra vector methods
  for(size_t i = 0; i < ncols; i++) {
    (*vec)->replaceLocalValue(i,values[i]);
   // std::cout << "[" << i << "] Write value : " << values[i] << std::endl;
  }

 /*auto trilinos_vec = (*vec)->getVectorNonConst(0);
  trilinos_vec->getLocalViewHost(Tpetra::Access::OverwriteAll);
  for(size_t i = 0; i < ncols; i++) {
    trilinos_vec->replaceLocalValue(i,values[i]);
  }*/

  // Tpetra, data view methods
  /*{
    Teuchos::ArrayRCP<SC> vec_data = (*vec)->get1dViewNonConst();
    const size_t localLength = (*vec)->getLocalLength ();
    for (size_t k = 0; k < localLength; ++k) {
      // Add k (the local index) to every entry of x.  Treat
      // scalar_type as a function to convert k to scalar_type.
      vec_data[k] = SC (values[k]);
    }
  }*/

  // Kokkos, view methods
  /*{
    auto x_2d = (*vec)->getLocalViewHost(Tpetra::Access::ReadWrite);
    auto x_1d = Kokkos::subview(x_2d, Kokkos::ALL(), 0);

    const size_t localLength = (*vec)->getLocalLength ();
    for (size_t k = 0; k < localLength; ++k) {
      // Add k (the local index) to every entry of x.  Treat 'double'
      // as a function to convert k (an integer) to double.
      x_1d(k) = SC(values[k]);
    }
  }*/


 /*auto data = (*vec)->getDataNonConst();
  for(size_t i = 0; i < ncols; i++) {
   // std::cout << "[" << i << "] After Read value : " << data[i] << std::endl;
  }*/

  std::cout << std::endl << " -------------------------- >>> exit VEC set Values " << vec << std::endl;

}

void Vector::getValues(Arccore::ArrayView<double> values) const
{

  std::cout << std::endl << " -------------------------- >>> enter VEC get Values !!! " << vec << std::endl;

  auto trilinos_vec = (*vec)->getDataNonConst();
  //auto read_h =  (*vec)->getLocalViewDevice(Tpetra::Access::ReadOnly);
  //auto read_h =  (*vec)->getLocalViewHost(Tpetra::Access::ReadOnly);
  //Teuchos::ArrayRCP<const SC> data = (*vec)->get1dView ();
  //auto trilinos_vec = (*vec)->getLocalViewDevice(Tpetra::Access::ReadOnly);

  /*const auto localOffset = distribution().offset();
  const auto localSize = distribution().localSize();

  for (int i =0; i<localSize; i++)
    std::cout << trilinos_vec.view(localOffset,localSize)[i];*/


  auto csr_cols = values.size();
  auto trilinos_cols = trilinos_vec.size();

 // std::cout << "csr_cols : " << csr_cols  << std::endl; // local
 // std::cout << "trilinos_cols : " << trilinos_cols  << std::endl; // global
  //std::cout << "get_local_length : " << (*vec)->getLocalLength()  << std::endl; // global
  //std::cout << "getGlobalLength : " << (*vec)->getGlobalLength()  << std::endl; // global



  if (csr_cols != trilinos_cols) {
    throw Arccore::FatalErrorException(A_FUNCINFO, "sizes are not equal");
  }

  for(size_t i = 0; i < csr_cols; i++) {
    values[i]=trilinos_vec[i];
  }
  /*for(size_t i = 0; i < ncols; i++) {
  //  std::cout << "trilinos_vec " << i << " : " << trilinos_vec[i] << std::endl;
    //std::cout << "values " << i << " : " << values[i] << std::endl;
  }*/
}

void Vector::assemble()
{
  // not needed ?
}

} // namespace Alien::Hypre