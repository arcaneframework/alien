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

#include "matrix.h"

#include <alien/ginkgo/backend.h>
#include <alien/core/impl/MultiMatrixImpl.h>

#include <arccore/message_passing_mpi/MpiMessagePassingMng.h>
#include <iostream>
namespace Alien::Ginkgo
{

Matrix::Matrix(const MultiMatrixImpl* multi_impl)
: IMatrixImpl(multi_impl, AlgebraTraits<BackEnd::tag::ginkgo>::name())
, gko::matrix::Csr<double, int>(
  gko::ReferenceExecutor::create(),
  gko::dim<2>(multi_impl->rowSpace().size(), multi_impl->colSpace().size()))
, data(gko::dim<2>{ (multi_impl->rowSpace().size(), multi_impl->colSpace().size()) })
{
  const auto& row_space = multi_impl->rowSpace();
  const auto& col_space = multi_impl->colSpace();

  /* alien_debug([&] {
          cout() << "[NM========================================\n"
                 << "row_space.size() "
                 << " : " << row_space.size()
                 << "\ncol_space.size() "
                 << " : " << col_space.size()
                 << "\n=========================================================NM]";
        });*/

  // Checks that the matrix is square
  if (row_space.size() != col_space.size())
    throw Arccore::FatalErrorException("Matrix must be square");
}

Matrix::~Matrix()
{
  /*    if (m_mat)
            MatDestroy(&m_mat);           */
}

/*void Matrix::setProfile(
            int ilower, int iupper, int jlower, int jupper,
            [[maybe_unused]] Arccore::ConstArrayView<int> row_sizes) {
    }*/

void Matrix::assemble()
{
  this->read(data);
}

void Matrix::setRowValues(int row, Arccore::ConstArrayView<int> cols, Arccore::ConstArrayView<double> values)
{
  auto ncols = cols.size();
  if (ncols != values.size()) {
    throw Arccore::FatalErrorException(A_FUNCINFO, "sizes are not equal");
  }

  std::clog << "[NM==========================CALL to  setRowValues ==============, row : " << row << "\n";

  for (auto icol = 0; icol < ncols; ++icol) {
    //std::clog << "data.add_value : row : " << row << " icol : " << cols[icol] << " - value : " << values[icol] << "\n";
    data.add_value(row, cols[icol], values[icol]);
  }
}

} // namespace Alien::Ginkgo
