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

#include <alien/core/impl/IMatrixImpl.h>
#include <alien/trilinos/backend.h>
#include <Tpetra_CrsMatrix.hpp>
#include <Tpetra_Core.hpp>
#include <Teuchos_ParameterXMLFileReader.hpp>
#include <Teuchos_TimeMonitor.hpp>
#include <Teuchos_DefaultMpiComm.hpp> // wrapper for MPI_Comm

namespace Alien::Trilinos
{
class Matrix : public IMatrixImpl
{
  // typedefs
  typedef Kokkos::Compat::KokkosOpenMPWrapperNode Node;
  typedef double                                          SC;
  typedef typename Tpetra::Map<>::local_ordinal_type      LO;
  typedef typename Tpetra::Map<>::global_ordinal_type     GO;
  typedef Tpetra::CrsMatrix<SC,LO,GO,Node>                crs_matrix_type;
  typedef Tpetra::Map<LO,GO,Node>                         map_type;


 public:
  explicit Matrix(const MultiMatrixImpl* multi_impl);

  virtual ~Matrix();

  void setProfile(int ilower, int iupper,
                  int numLocalRows,
                  int numGlobalRows,
                  const Arccore::UniqueArray<int> & rowSizes);

  void setRowValues(int rows,
                    Arccore::ConstArrayView<int> cols,
                    Arccore::ConstArrayView<double> values);

  void assemble();

  Teuchos::RCP<crs_matrix_type> const &  internal() const { return mtx; }
  Teuchos::RCP<crs_matrix_type> & internal() { return mtx; }

 private:
  Teuchos::RCP<crs_matrix_type> mtx;
  Teuchos::RCP<const Teuchos::Comm<int>> t_comm;
};

} // namespace Alien::Trilinos
