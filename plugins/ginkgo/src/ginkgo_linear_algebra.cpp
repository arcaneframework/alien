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
#include "vector.h"

#include <arccore/message_passing_mpi/MpiMessagePassingMng.h>
#include <alien/core/backend/LinearAlgebraT.h>

#include <alien/ginkgo/backend.h>
#include <alien/ginkgo/export.h>

#include <ginkgo/ginkgo.hpp>

namespace Alien
{
template class ALIEN_GINKGO_EXPORT LinearAlgebra<BackEnd::tag::ginkgo>;
} // namespace Alien

namespace Alien::Ginkgo
{
class ALIEN_GINKGO_EXPORT InternalLinearAlgebra
: public IInternalLinearAlgebra<Matrix, Vector>
{
 public:
  InternalLinearAlgebra() = default;

  ~InternalLinearAlgebra() override = default;

 public:
  // IInternalLinearAlgebra interface.
  Arccore::Real norm0(const Vector& x) const override;

  Arccore::Real norm1(const Vector& x) const override;

  Arccore::Real norm2(const Vector& x) const override;

  void mult(const Matrix& a, const Vector& x, Vector& r) const override;

  void axpy(Arccore::Real alpha, const Vector& x,
            Vector& r) const override;

  void aypx(Arccore::Real alpha, Vector& y,
            const Vector& x) const override;

  void copy(const Vector& x, Vector& r) const override;

  Arccore::Real dot(const Vector& x, const Vector& y) const override;

  void scal(Arccore::Real alpha, Vector& x) const override;

  void diagonal(const Matrix& a, Vector& x) const override;

  void reciprocal(Vector& x) const override;

  void pointwiseMult(const Vector& x, const Vector& y,
                     Vector& w) const override;
};

Arccore::Real
InternalLinearAlgebra::norm0(const Vector& vx ALIEN_UNUSED_PARAM) const
{
  //     throw Arccore::NotImplementedException(A_FUNCINFO, "PetscLinearAlgebra::norm0 not implemented");
  return {};
}

Arccore::Real
InternalLinearAlgebra::norm1(const Vector& vx) const
{
  //~ PetscScalar norm;
  //~ VecNorm(vx.internal(), NORM_1, &norm);
  //~ return static_cast<Arccore::Real>(norm);
  return {};
}

Arccore::Real
InternalLinearAlgebra::norm2(const Vector& vx) const
{
  //~ PetscScalar norm;
  //~ VecNorm(vx.internal(), NORM_2, &norm);
  //~ return static_cast<Arccore::Real>(norm);
  return {};
}

void InternalLinearAlgebra::mult(const Matrix& ma, const Vector& vx, Vector& vr) const
{
  /*std::clog << "vx : " << vx.space().size();
  std::clog << "vr : " << vr.space().size();
  std::clog << "g vx : " << vx.internal()->get_size()[0] << " - " << vx.internal()->get_size()[1] << "\n" ;
  std::clog << "g vx : " << vx.internal()->get_size()[0] << " - " << vx.internal()->get_size()[1] << "\n" ;*/

  /* Debug MATRIX */
  /*std::cout << "=== MATMULT : DEBUG ma " << std::endl;
  std::cout << "=== MATMULT : DEBUG ma get num stored elements : " << ma.get_num_stored_elements() << std::endl;
  std::cout << "=== MATMULT : DEBUG ma size L x C : " << ma.get_size()[0] << " x " << ma.get_size()[1] << std::endl;
  std::cout << "=== MATMULT : Affichage ligne par ligne  : " << std::endl;
  int nRows = ma.get_size()[0] ;
  int nCols = ma.get_size()[1] ;

  auto vals = ma.get_const_values();
  auto cols = ma.get_const_col_idxs();
  auto rows = ma.get_const_row_ptrs();

  // check data in matrix ma
  for (int i = 0; i<nRows; i++)
  {
    int nbValuesRow = rows[i+1] - rows[i];
    std::cout << "row : " << i << " - nbValues : " << nbValuesRow << " || ";
    for (int j =0; j<nbValuesRow; j++)
    {
      std::cout << vals[rows[i]+j] <<" ";
    }
    std::cout <<" cols : ";
    for (int j =0; j<nbValuesRow; j++)
    {
      std::cout << cols[rows[i]+j] << " ";
    }
    std::cout << "\n";
  }

  //check data in vector vx
  std::cout << "=== MATMULT : DEBUG VX size L x C : " << vx.get_size()[0] << " x " << vx.get_size()[1] << std::endl;
  auto vxVal = vx.get_const_values();
  for (int i =0; i<vx.get_size()[0]; i++)
  {
    std::cout << vxVal[i] <<" ";
  }*/

  ma.internal()->apply(lend(vx.internal()), lend(vr.internal()));

  //check data in vector vx
  /*std::cout << "\n=== MATMULT : DEBUG VR size L x C : " << vr.get_size()[0] << " x " << vr.get_size()[1] << std::endl;
  auto vrVal = vr.get_const_values();
  for (int i =0; i<vr.get_size()[0]; i++)
  {
    std::cout << vrVal[i] <<" ";
  }*/
}

void InternalLinearAlgebra::axpy(
Arccore::Real alpha, const Vector& vx, Vector& vr) const
{
  //~ VecAXPY(vr.internal(), alpha, vx.internal()); //  vr = alpha.vx + vr
}

void InternalLinearAlgebra::copy(const Vector& vx, Vector& vr) const
{
  //~ VecCopy(vx.internal(), vr.internal());
}

Arccore::Real
InternalLinearAlgebra::dot(const Vector& vx, const Vector& vy) const
{
  //~ PetscScalar dot_prod;
  //~ VecDot(vx.internal(), vy.internal(), &dot_prod);
  //~ return static_cast<Arccore::Real>(dot_prod);
  return {};
}

void InternalLinearAlgebra::diagonal(Matrix const& m ALIEN_UNUSED_PARAM, Vector& v ALIEN_UNUSED_PARAM) const
{
  //~ MatGetDiagonal(m.internal(), v.internal());
  // return{};
}

void InternalLinearAlgebra::reciprocal(Vector& v ALIEN_UNUSED_PARAM) const
{
  //~ VecReciprocal(v.internal());
}

void InternalLinearAlgebra::aypx(
Arccore::Real alpha, Vector& y, const Vector& x) const
{
  //~ VecAYPX(y.internal(), alpha, x.internal()); // y = x + alpha y
}

void InternalLinearAlgebra::pointwiseMult(
const Vector& x, const Vector& y, Vector& w) const
{
  //VecPointwiseMult(
  //~ w.internal(), x.internal(),
  //~ y.internal()); // Computes the componentwise multiplication w = x*y.
}

void InternalLinearAlgebra::scal(Arccore::Real alpha, Vector& x) const
{
  //   VecScale(x.internal(), alpha);
}

ALIEN_GINKGO_EXPORT
IInternalLinearAlgebra<Ginkgo::Matrix, Ginkgo::Vector>*
InternalLinearAlgebraFactory()
{
  return new Ginkgo::InternalLinearAlgebra();
}
} // namespace Alien::Ginkgo
