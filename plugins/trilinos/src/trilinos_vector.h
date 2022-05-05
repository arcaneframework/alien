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

#pragma once

#include <alien/core/impl/IVectorImpl.h>
#include <Tpetra_Core.hpp>
#include <Tpetra_CrsMatrix.hpp> // en inclure moins ?
#include <Teuchos_DefaultMpiComm.hpp> // wrapper for MPI_Comm

namespace Alien::Trilinos
{
class VectorInternal;

class Vector : public IVectorImpl
{
  // typedefs
  typedef Kokkos::Compat::KokkosOpenMPWrapperNode Node;
  typedef double                                          SC;
  typedef typename Tpetra::Map<>::local_ordinal_type      LO;
  typedef typename Tpetra::Map<>::global_ordinal_type     GO;
  typedef Tpetra::Vector<SC, LO, GO, Node>                vec_type;


 public:
  explicit Vector(const MultiVectorImpl* multi_impl);

  virtual ~Vector();

 public:
  void setProfile(int ilower, int iupper,int numGlobalElts, int numLocalElts);

  void setValues(Arccore::ConstArrayView<double> values);

  void getValues(Arccore::ArrayView<double> values) const;

  void assemble();

  //HYPRE_IJVector internal() { return m_hypre; }
  //HYPRE_IJVector internal() const { return m_hypre; }

  Teuchos::RCP<vec_type> internal() const { return *vec; }
  Teuchos::RCP<vec_type> * ptr() const { return vec; }


 private:
  //std::unique_ptr<Teuchos::RCP<vec_type>> vec;
  Teuchos::RCP<vec_type> * vec;

  Teuchos::RCP<const Teuchos::Comm<int>> t_comm;
  //HYPRE_IJVector m_hypre;
  //MPI_Comm m_comm;

  //Arccore::UniqueArray<Arccore::Integer> m_rows;
};

} // namespace Alien::Hypre
