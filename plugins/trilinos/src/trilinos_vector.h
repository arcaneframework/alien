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

#include <alien/trilinos/backend.h>

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
  typedef Kokkos::Compat::KokkosOpenMPWrapperNode         Node;
  typedef double                                          SC;
  typedef typename Tpetra::Map<>::local_ordinal_type      LO;
  typedef typename Tpetra::Map<>::global_ordinal_type     GO;
  typedef Tpetra::MultiVector<SC, LO, GO, Node>           MV;
  typedef Tpetra::Map<LO,GO,Node>                         map_type;

 public:
  explicit Vector(const MultiVectorImpl* multi_impl);

  virtual ~Vector();

 public:
  void setProfile(int ilower, int iupper,int numGlobalElts, int numLocalElts);

  void setValues(Arccore::ConstArrayView<double> values);

  void getValues(Arccore::ArrayView<double> values) const;

  Teuchos::RCP<MV> const & internal() const { return vec; }
  Teuchos::RCP<MV> & internal() { return vec; }

 private:
  Teuchos::RCP<MV> vec;
  Teuchos::RCP<const Teuchos::Comm<int>> t_comm;

};

} // namespace Alien::Trilinos
