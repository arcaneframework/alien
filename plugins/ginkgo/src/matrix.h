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

#pragma once
#include <memory>
#include <alien/core/impl/IMatrixImpl.h>

//#include <petscmat.h>


namespace Alien::Ginkgo
{
class Matrix : public IMatrixImpl
, public gko::matrix::Csr<double, int>
{
 public:
  explicit Matrix(const MultiMatrixImpl* multi_impl);

  ~Matrix() override;

 public:
  /* void setProfile(int ilower, int iupper, int jlower, int jupper,
                        [[maybe_unused]] Arccore::ConstArrayView<int> row_sizes);*/

  void setRowValues(int rows,
                    Arccore::ConstArrayView<int> cols,
                    Arccore::ConstArrayView<double>);

  void assemble();

/* make_shared does not work : loses the pointer to the gko::executor !
  std::shared_ptr<gko::matrix::Csr<double, int>> internal() const {
    return std::make_shared<gko::matrix::Csr<double, int>>(*this); }
  std::shared_ptr<gko::matrix::Csr<double, int>> internal() {
    return std::make_shared<gko::matrix::Csr<double, int>>(*this); }
*/

  /* QUESTION : Return a shared_ptr ?
  std::shared_ptr<gko::matrix::Csr<double, int>> & internal() const{
    return std::shared_ptr<gko::matrix::Csr<double, int>>(this);
  }

  /*std::shared_ptr<gko::matrix::Csr<double, int>> & internal(){
    return {this};
    }
*/

  /* Return a raw pointer */
  gko::matrix::Csr<double, int> const* internal() const{
    return this;
  }

  gko::matrix::Csr<double, int>* internal(){
    return this;
  }

 private:
  /*using ValueType = double;
     using IndexType = int;
     using mtx = gko::matrix::Csr<ValueType, IndexType>;
     mtx m_mat;*/
  gko::matrix_assembly_data<double, int> data;
};

} // namespace Alien::Ginkgo
