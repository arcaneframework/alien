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

#include "hypre_matrix.h"
#include "hypre_instance.h"
#include "hypre_vector.h"

#include <alien/hypre/backend.h>
#include <alien/core/impl/MultiMatrixImpl.h>
#include <alien/data/ISpace.h>

#include <arccore/message_passing_mpi/MpiMessagePassingMng.h>

#include <HYPRE.h>
// For hypre_*Alloc
#include <_hypre_utilities.h>
#include <numeric>

#ifdef HAVE_HYPRE_BIGINT
using HypreId = HYPRE_BigInt;
#else
using HypreId = HYPRE_Int;
#endif

namespace Alien::Hypre
{
Matrix::Matrix(const MultiMatrixImpl* multi_impl)
: IMatrixImpl(multi_impl, AlgebraTraits<BackEnd::tag::hypre>::name())
{
  auto const* pm = dynamic_cast<Arccore::MessagePassing::Mpi::MpiMessagePassingMng*>(multi_impl->distribution().parallelMng());
  m_comm = pm ? (*pm->getMPIComm()) : MPI_COMM_WORLD;

  hypre_init_if_needed(m_comm);
  const auto& row_space = multi_impl->rowSpace();
  const auto& col_space = multi_impl->colSpace();
  if (row_space.size() != col_space.size())
    throw Arccore::FatalErrorException("Hypre matrix must be square");
}

Matrix::~Matrix()
{
  if (m_hypre)
    HYPRE_IJMatrixDestroy(m_hypre);
}

void Matrix::setProfile(int ilower, int iupper,
                        int jlower, int jupper,
                        Arccore::ConstArrayView<int> row_sizes)
{
  if (m_hypre)
    HYPRE_IJMatrixDestroy(m_hypre);

  auto ierr = HYPRE_IJMatrixCreate(m_comm, ilower, iupper, jlower, jupper, &m_hypre);
  ierr |= HYPRE_IJMatrixSetObjectType(m_hypre, HYPRE_PARCSR);
  ierr |= HYPRE_IJMatrixInitialize(m_hypre);
  ierr |= HYPRE_IJMatrixSetRowSizes(m_hypre, row_sizes.data());

  if (ierr) {
    throw Arccore::FatalErrorException(A_FUNCINFO, "Hypre Initialisation failed");
  }
}

void Matrix::assemble()
{
  auto ierr = HYPRE_IJMatrixAssemble(m_hypre);

  if (ierr) {
    throw Arccore::FatalErrorException(A_FUNCINFO, "Hypre assembling failed");
  }
}

void Matrix::setRowValues(int row, Arccore::ConstArrayView<int> cols, Arccore::ConstArrayView<double> values)
{
  int one = 1;
  auto rows = ArrayView<int>(1, &row);
  auto ncols = ArrayView<int>(1, &one);

  return setRowsValues(rows, ncols, cols, values);
}

void Matrix::setRowsValues(Arccore::ConstArrayView<int> rows, Arccore::ArrayView<int> ncols, Arccore::ConstArrayView<int> cols, Arccore::ConstArrayView<double> values)
{
  // First, check parameters consistency
  if (rows.size() != ncols.size()) {
    throw Arccore::FatalErrorException(A_FUNCINFO, "rows and n_cols should have the same size");
  }
  if (cols.size() != values.size()) {
    throw Arccore::FatalErrorException(A_FUNCINFO, "cols and values should have the same size");
  }
  if (auto g_size = std::accumulate(ncols.begin(), ncols.end(), 0); g_size != cols.size()) {
    throw Arccore::FatalErrorException(A_FUNCINFO, "Sum(ncols) != cols.size()");
  }

  // Second, create hypre data accessor
  const HypreId* hypre_cols = nullptr;
  const HYPRE_Real* hypre_values = nullptr;
  HYPRE_Int* hypre_ncols = nullptr;
  const HypreId* hypre_rows = nullptr;

#ifdef ALIEN_HYPRE_DEVICE
  HYPRE_MemoryLocation memory_location;
  HYPRE_GetMemoryLocation(&memory_location);
  if (memory_location != HYPRE_MEMORY_HOST) {
    HypreId* d_cols = hypre_CTAlloc(HypreId, cols.size(), memory_location);
    HYPRE_Real* d_values = hypre_CTAlloc(HYPRE_Real, values.size(), memory_location);
    HYPRE_Int* d_ncols = hypre_CTAlloc(HYPRE_Int, ncols.size(), memory_location);
    HypreId* d_rows = hypre_CTAlloc(HypreId, rows.size(), memory_location);

    hypre_TMemcpy(d_cols, cols.data(), HypreId, cols.size(), memory_location, HYPRE_MEMORY_HOST);
    hypre_TMemcpy(d_values, values.data(), HYPRE_Real, values.size(), memory_location, HYPRE_MEMORY_HOST);
    hypre_TMemcpy(d_ncols, ncols.data(), HYPRE_Int, ncols.size(), memory_location, HYPRE_MEMORY_HOST);
    hypre_TMemcpy(d_rows, rows.data(), HypreId, rows.size(), memory_location, HYPRE_MEMORY_HOST);

    hypre_cols = d_cols;
    hypre_values = d_values;
    hypre_ncols = d_ncols;
    hypre_rows = d_rows;
  }
  else
#endif // ALIEN_HYPRE_DEVICE
  {
    hypre_cols = cols.data();
    hypre_values = values.data();
    hypre_ncols = ncols.data();
    hypre_rows = rows.data();
  }

  // Third call hypre.
  auto ierr = HYPRE_IJMatrixSetValues(m_hypre, rows.size(), hypre_ncols, hypre_rows, hypre_cols, hypre_values);

  if (ierr) {
    auto msg = Arccore::String::format("Cannot set Hypre Matrix Values");
    throw Arccore::FatalErrorException(A_FUNCINFO, msg);
  }
#ifdef ALIEN_HYPRE_DEVICE
  if (memory_location != HYPRE_MEMORY_HOST) {
    hypre_TFree(hypre_rows, memory_location);
    hypre_TFree(hypre_ncols, memory_location);
    hypre_TFree(hypre_values, memory_location);
    hypre_TFree(hypre_cols, memory_location);
  }
#endif // ALIEN_HYPRE_DEVICE
}

} // namespace Alien::Hypre