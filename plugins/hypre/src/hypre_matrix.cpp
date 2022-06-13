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

#include <alien/hypre/backend.h>
#include <alien/core/impl/MultiMatrixImpl.h>
#include <alien/data/ISpace.h>

#include <arccore/message_passing_mpi/MpiMessagePassingMng.h>

#include <HYPRE.h>

namespace Alien
{
Matrix::Matrix(const MultiMatrixImpl* multi_impl)
: IMatrixImpl(multi_impl, "hypre")
, m_hypre(nullptr)
{
  const auto& row_space = multi_impl->rowSpace();
  const auto& col_space = multi_impl->colSpace();
  if (row_space.size() != col_space.size())
    throw Arccore::FatalErrorException("Hypre matrix must be square");

  auto* pm = dynamic_cast<Arccore::MessagePassing::Mpi::MpiMessagePassingMng*>(distribution().parallelMng());
  m_comm = pm ? (*pm->getMPIComm()) : MPI_COMM_WORLD;
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

void Matrix::setRowValues(int rows, Arccore::ConstArrayView<int> cols, Arccore::ConstArrayView<double> values)
{
  auto ncols = cols.size();

  if (ncols != values.size()) {
    throw Arccore::FatalErrorException(A_FUNCINFO, "sizes are not equal");
  }

  auto ierr = HYPRE_IJMatrixSetValues(m_hypre, 1, &ncols, &rows, cols.data(), values.data());

  if (ierr) {
    auto msg = Arccore::String::format("Cannot set Hypre Matrix Values for row {0}", rows);
    throw Arccore::FatalErrorException(A_FUNCINFO, msg);
  }
}

} // namespace Alien
