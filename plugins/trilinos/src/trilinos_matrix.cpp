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

#include "trilinos_matrix.h"

#include <alien/trilinos/backend.h>
#include <alien/core/impl/MultiMatrixImpl.h>
#include <alien/data/ISpace.h>

#include <arccore/message_passing_mpi/MpiMessagePassingMng.h>

namespace Alien::Trilinos
{
Matrix::Matrix(const MultiMatrixImpl* multi_impl)
: IMatrixImpl(multi_impl, AlgebraTraits<BackEnd::tag::trilinos>::name())
, mtx(nullptr)
{
  std::cout << " -------------------------- >>> ctor !!! " << std::endl;

  // Checks that the matrix is square
  const auto& row_space = multi_impl->rowSpace();
  const auto& col_space = multi_impl->colSpace();
  if (row_space.size() != col_space.size())
    throw Arccore::FatalErrorException("Matrix must be square");

  // communicator
  using Teuchos::RCP;
  using Teuchos::Comm;
  using Teuchos::MpiComm;
  MPI_Comm yourComm = MPI_COMM_WORLD;
  t_comm = RCP<const Comm<int>>(new MpiComm<int> (yourComm)); // Récupérer le communicateur Arcane ?

}

Matrix::~Matrix()
{
  if (mtx)
    mtx.release();
}

void Matrix::setProfile(int ilower, int iupper,
                        int numLocalRows,
                        int numGlobalRows,
                        const Arccore::UniqueArray<int> & rowSizes)
{
  std::cout << " -------------------------- >>> set profile !!! " << std::endl;

  using Teuchos::RCP;
  using Teuchos::rcp;
  typedef Kokkos::Compat::KokkosOpenMPWrapperNode Node;
  typedef typename Tpetra::Map<>::local_ordinal_type      LO;
  typedef typename Tpetra::Map<>::global_ordinal_type     GO;
  typedef Tpetra::Map<LO,GO,Node>                         map_type;

  //if already exists, dealloc
  if (mtx)
    mtx.release();

  // map
  RCP<const map_type> rowMap = rcp (new map_type (numGlobalRows,numLocalRows,0,t_comm));

  // matrix
  Teuchos::Array<size_t> entriesPerRow(numLocalRows);
  for(size_t i = 0; i < numLocalRows; i++)
    entriesPerRow[i] = rowSizes[i];

  RCP<crs_matrix_type> A (new crs_matrix_type (rowMap, entriesPerRow()));
  mtx = std::make_unique<Teuchos::RCP<crs_matrix_type>> (new crs_matrix_type (rowMap, entriesPerRow()));


}

void Matrix::assemble()
{
  (*mtx)->fillComplete();
  std::cout << "Fill Completed ! " << std::endl;

}

void Matrix::setRowValues(int row, Arccore::ConstArrayView<int> columns, Arccore::ConstArrayView<double> values)
{
  auto ncols = columns.size();

  if (ncols != values.size()) {
    throw Arccore::FatalErrorException(A_FUNCINFO, "sizes are not equal");
  }

  std::cout << "row : " << row << std::endl;

  Teuchos::Array<SC> vals(ncols);
  Teuchos::Array<GO> cols(ncols);

  for(size_t i = 0; i < ncols; i++) {
    cols[i] = columns[i];
    vals[i] = values[i];
  }

  auto valsView = vals();
  auto colsView = cols();

  (*mtx)->insertGlobalValues(row, ncols, values.data(), cols.data()/*reinterpret_cast<const long long int*>(columns.data())*/); // insertLocal possible but needs colmap
  std::cout << "row : " << row << " inserted ! " << std::endl;

}

} // namespace Alien::Trilinos