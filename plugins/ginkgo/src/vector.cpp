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

#include "vector.h"

#include <alien/ginkgo/backend.h>
#include <alien/ginkgo/machine_backend.h>
#include <alien/core/impl/MultiVectorImpl.h>

namespace Alien::Ginkgo
{
Vector::Vector(const MultiVectorImpl* multi_impl)
: IVectorImpl(multi_impl, AlgebraTraits<BackEnd::tag::ginkgo>::name())
, gko::matrix::Dense<double>(
  ginkgo_executor::exec_map.at(ginkgo_executor::target_machine)(), // throws if not valid
  // Alien::Ginkgo::create<exec_target>(),
  //gko::ReferenceExecutor::create(),
  //gko::CudaExecutor::create(0, gko::OmpExecutor::create(),true),
  gko::dim<2>(multi_impl->space().size(), 1))
, data(gko::dim<2>(multi_impl->space().size(), 1))
{
  alien_debug([&] {
    cout() << "Vector size : "
           << multi_impl->space().size();
    cout() << "data size : "
           << data.get_size()[0] << "row "
           << " - " << data.get_size()[0] << " cols";
  });
}

Vector::~Vector()
{
  /*if (m_vec)
            VecDestroy(&m_vec);*/
}

void Vector::setProfile(int ilower, int iupper)
{
}

void Vector::setValues(Arccore::ConstArrayView<double> values)
{
  auto ncols = values.size();

  //std::clog << "[NM==========================CALL to  setValues ============== with : " << ncols << " values.\n";

  for (auto icol = 0; icol < ncols; ++icol) {
    //std::clog << "data.add_value icol : " << icol << " - value : " << values[icol] << "\n";
    //data.add_value(0, icol, values[icol]);
    data.add_value(icol, 0, values[icol]);
  }
  //std::clog << "[NM==========================data contains ==============, with : " << data.get_size()[0] << " rows - " << data.get_size()[1] << " cols \n";

  /*for (int i =0; i<data.get_size()[0]; i++)
  {
    std::cout << data.get_value(i,0) <<" ";
  }*/
}

void Vector::getValues(Arccore::ArrayView<double> values) const
{
  /*auto ierr = VecGetValues(m_vec, m_rows.size(), m_rows.data(), values.data());

        if (ierr) {
            throw Arccore::FatalErrorException(A_FUNCINFO, "PETSc Vector get values failed");
        }*/
}

void Vector::assemble()
{
  if ((this->get_size()[0] == data.get_size()[0]) && (this->get_size()[1] == data.get_size()[1])) {
    this->read(data);
  }
  else
    throw Arccore::FatalErrorException("Vec size does not match data size");
  /*std::cout << "\n------------ data contains : -----------------" << std::endl;
  for (int i = 0; i<data.get_size()[0]; i++) {
    std::cout << data.get_value(i, 0) << " ";
  }
  std::cout << "\n------------ gko::matrix::Dense contains : -----------------" << std::endl;

  std::cout << "num stored elements : " << this->get_num_stored_elements() << "\n";
  auto vals = this->get_values();

  for (int i = 0;  i < 10; i++) {
      std::cout << vals[i] << " ";
  }*/
}
} // namespace Alien::Ginkgo
