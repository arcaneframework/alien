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

#include "trilinos_matrix.h"
#include "trilinos_vector.h"

#include <cmath>



#include <arccore/message_passing_mpi/MpiMessagePassingMng.h>
#include <arccore/base/NotImplementedException.h>
#include <arccore/base/TraceInfo.h>

#include <alien/core/backend/LinearAlgebraT.h>

#include <alien/trilinos/backend.h>
#include <alien/trilinos/export.h>

namespace Alien
{
template class ALIEN_TRILINOS_EXPORT LinearAlgebra<BackEnd::tag::trilinos>;
} // namespace Alien

namespace Alien::Trilinos
{
class ALIEN_TRILINOS_EXPORT InternalLinearAlgebra
: public IInternalLinearAlgebra<Matrix, Vector>
{
 public:
  InternalLinearAlgebra() {}

  virtual ~InternalLinearAlgebra() {}

 public:
  // IInternalLinearAlgebra interface.
  Arccore::Real norm0(const Vector& x) const override;

  Arccore::Real norm1(const Vector& x) const override;

  Arccore::Real norm2(const Vector& x) const override;

  void mult(const Matrix& a, const Vector& x, Vector& r) const override;

  void axpy(Arccore::Real alpha, const Vector& x, Vector& r) const override;

  void aypx(Arccore::Real alpha, Vector& y, const Vector& x) const override;

  void copy(const Vector& x, Vector& r) const override;

  Arccore::Real dot(const Vector& x, const Vector& y) const override;

  void scal(Arccore::Real alpha, Vector& x) const override;

  void diagonal(const Matrix& a, Vector& x) const override;

  void reciprocal(Vector& x) const override;

  void pointwiseMult(const Vector& x, const Vector& y, Vector& w) const override;
};

Arccore::Real
InternalLinearAlgebra::norm0(const Vector& vx ALIEN_UNUSED_PARAM) const
{
  //throw Arccore::NotImplementedException(A_FUNCINFO, "HypreLinearAlgebra::norm0 not implemented");
}

Arccore::Real
InternalLinearAlgebra::norm1(const Vector& vx ALIEN_UNUSED_PARAM) const
{
  //throw Arccore::NotImplementedException(A_FUNCINFO, "HypreLinearAlgebra::norm1 not implemented");
}

Arccore::Real
InternalLinearAlgebra::norm2(const Vector& vx) const
{
  //return std::sqrt(dot(vx, vx));
}

void InternalLinearAlgebra::mult(const Matrix& ma, const Vector& vx, Vector& vr) const
{
  //HYPRE_ParCSRMatrixMatvec(1.0, hypre_implem(ma), hypre_implem(vx), 0.0, hypre_implem(vr));
}

void InternalLinearAlgebra::axpy(
Arccore::Real alpha, const Vector& vx, Vector& vr) const
{
  //HYPRE_ParVectorAxpy(alpha, hypre_implem(vx), hypre_implem(vr));
}

void InternalLinearAlgebra::copy(const Vector& vx, Vector& vr) const
{
  //HYPRE_ParVectorCopy(hypre_implem(vx), hypre_implem(vr));
}

Arccore::Real
InternalLinearAlgebra::dot(const Vector& vx, const Vector& vy) const
{
  /*double dot_prod = 0;
  HYPRE_ParVectorInnerProd(hypre_implem(vx), hypre_implem(vy), &dot_prod);
  return static_cast<Arccore::Real>(dot_prod);*/
}

void InternalLinearAlgebra::diagonal(Matrix const& m ALIEN_UNUSED_PARAM, Vector& v ALIEN_UNUSED_PARAM) const
{
  /*throw Arccore::NotImplementedException(
  A_FUNCINFO, "HypreLinearAlgebra::diagonal not implemented");*/
}

void InternalLinearAlgebra::reciprocal(Vector& v ALIEN_UNUSED_PARAM) const
{
  /*throw Arccore::NotImplementedException(
  A_FUNCINFO, "HypreLinearAlgebra::reciprocal not implemented");*/
}

void InternalLinearAlgebra::aypx(
Arccore::Real alpha ALIEN_UNUSED_PARAM, Vector& y ALIEN_UNUSED_PARAM, const Vector& x ALIEN_UNUSED_PARAM) const
{
  //throw Arccore::NotImplementedException(A_FUNCINFO, "HypreLinearAlgebra::aypx not implemented");
}

void InternalLinearAlgebra::pointwiseMult(
const Vector& x ALIEN_UNUSED_PARAM, const Vector& y ALIEN_UNUSED_PARAM, Vector& w ALIEN_UNUSED_PARAM) const
{
  /*throw Arccore::NotImplementedException(
  A_FUNCINFO, "HypreLinearAlgebra::pointwiseMult not implemented");*/
}

void InternalLinearAlgebra::scal(Arccore::Real alpha, Vector& x) const
{
  //HYPRE_ParVectorScale(static_cast<double>(alpha), hypre_implem(x));
}

ALIEN_TRILINOS_EXPORT
IInternalLinearAlgebra<Trilinos::Matrix, Trilinos::Vector>*
InternalLinearAlgebraFactory()
{
  return new Trilinos::InternalLinearAlgebra();
}
} // namespace Alien::Hypre
