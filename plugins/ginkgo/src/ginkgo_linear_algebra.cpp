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

// Fix : Return value ?
Arccore::Real
InternalLinearAlgebra::norm0(const Vector& vx ALIEN_UNUSED_PARAM) const
{
  throw Arccore::NotImplementedException(A_FUNCINFO, "GinkgoLinearAlgebra::norm0 not implemented");
  return {};
}

// Fix : Return value ?
Arccore::Real
InternalLinearAlgebra::norm1(const Vector& vx) const
{
  throw Arccore::NotImplementedException(A_FUNCINFO, "GinkgoLinearAlgebra::norm1 not implemented");
  return {};
}

Arccore::Real
InternalLinearAlgebra::norm2(const Vector& vx) const
{
  using vec = gko::matrix::Dense<double>;
  auto exec = vx.internal()->get_executor();
  // Fix : find a better way to create the res matrix without initializing it with a dumb value !
  auto mtx_res = gko::initialize<vec>({15000.0}, exec);

  vx.internal()->compute_norm2(mtx_res.get());

  return mtx_res->get_values()[0];
}

void InternalLinearAlgebra::mult(const Matrix& ma, const Vector& vx, Vector& vr) const
{
  ma.internal()->apply(lend(vx.internal()), lend(vr.internal()));
}

void InternalLinearAlgebra::axpy(
Arccore::Real alpha, const Vector& vx, Vector& vr) const
{
  /* Ginkgo's add_scaled method :
   * Adds `b` scaled by `alpha` to the matrix (aka: BLAS axpy).
   * @param alpha  If alpha is 1x1 Dense matrix, the entire matrix is scaled by alpha. [...]
   * @param b  a matrix of the same dimension as this */

  using vec = gko::matrix::Dense<double>;
  auto exec = vx.internal()->get_executor();
  auto mtx_alpha = gko::initialize<vec>({alpha}, exec);

  vr.internal()->add_scaled(mtx_alpha.get(), vx.internal());
}


void InternalLinearAlgebra::copy(const Vector& vx, Vector& vr) const
{
  vr.internal()->copy_from(vx.internal());
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
