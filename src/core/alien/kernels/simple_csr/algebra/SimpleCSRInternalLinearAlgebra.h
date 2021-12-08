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

#include <alien/utils/Precomp.h>

#include <alien/core/backend/IInternalLinearAlgebraExprT.h>
#include <alien/core/backend/IInternalLinearAlgebraT.h>
#include <alien/kernels/simple_csr/SimpleCSRBackEnd.h>

#include <alien/distribution/VectorDistribution.h>
#include <alien/distribution/MatrixDistribution.h>

#include <alien/utils/ExceptionUtils.h>

/*---------------------------------------------------------------------------*/

namespace Alien
{

typedef AlgebraTraits<BackEnd::tag::simplecsr>::matrix_type CSRMatrix;
typedef AlgebraTraits<BackEnd::tag::simplecsr>::vector_type CSRVector;

class ALIEN_EXPORT SimpleCSRInternalLinearAlgebra : public IInternalLinearAlgebra<CSRMatrix, CSRVector>
{
 public:
  typedef BackEnd::tag::simplecsr BackEndType ;

  typedef VectorDistribution ResourceType ;

  class NullValueException
      : public Exception::NumericException
  {
  public :
    typedef Exception::NumericException BaseType ;
    NullValueException(std::string const& type)
    : BaseType(type,__LINE__)
  {}
  } ;


  SimpleCSRInternalLinearAlgebra();
  virtual ~SimpleCSRInternalLinearAlgebra();

 public:
  // IInternalLinearAlgebra interface.
  Real norm0(const Vector& x) const;
  Real norm1(const Vector& x) const;
  Real norm2(const Vector& x) const;
  void mult(const Matrix& a, const Vector& x, Vector& r) const;
  void axpy(Real alpha, const Vector& x, Vector& r) const;
  void aypx(Real alpha, Vector& y, const Vector& x) const;
  void copy(const Vector& x, Vector& r) const;
  Real dot(const Vector& x, const Vector& y) const;
  void scal(Real alpha, Vector& x) const;
  void diagonal(const Matrix& a, Vector& x) const;
  void reciprocal(Vector& x) const;
  void pointwiseMult(const Vector& x, const Vector& y, Vector& w) const;
  void assign(Vector& x, Real alpha) const;

  void computeInvDiag(const Matrix& a, Vector& inv_diag) const;

  template<typename LambdaT>
  void assign(Vector& x, LambdaT const& lambda) const
  {
    auto x_ptr = x.getDataPtr() ;
    for (Integer i = 0; i < x.getAllocSize(); ++i)
    {
      x_ptr[i] = lambda(i);
    }
  }

  template <typename PrecondT>
  void exec(PrecondT& precond, Vector const& x, Vector& y) {
    return precond.solve(*this,x,y) ;
  }

  static ResourceType const& resource(Matrix const& A) ;

  void allocate(ResourceType const& distribution,Vector& v) ;

  template<typename T0, typename ...T>
  void allocate(ResourceType const& distribution,T0& v0, T& ... args)
  {
     allocate(distribution,v0) ;
     allocate(distribution,args...) ;
  }

  void free(Vector& v) ;

  template<typename T0, typename ...T>
  void free(T0& v0, T& ... args)
  {
     free(v0) ;
     free(args...) ;
  }

 private:
  // No member.
};

class SimpleCSRInternalLinearAlgebraExpr
: public IInternalLinearAlgebraExpr<CSRMatrix, CSRVector>
{
 public:
  SimpleCSRInternalLinearAlgebraExpr();
  virtual ~SimpleCSRInternalLinearAlgebraExpr();

 public:
  // IInternalLinearAlgebra interface.
  Real norm0(const Vector& x) const;
  Real norm1(const Vector& x) const;
  Real norm2(const Vector& x) const;
  void mult(const Matrix& a, const Vector& x, Vector& r) const;
  void axpy(Real alpha, const Vector& x, Vector& r) const;
  void aypx(Real alpha, Vector& y, const Vector& x) const;
  void copy(const Vector& x, Vector& r) const;
  Real dot(const Vector& x, const Vector& y) const;
  void scal(Real alpha, Vector& x) const;
  void diagonal(const Matrix& a, Vector& x) const;
  void reciprocal(Vector& x) const;
  void pointwiseMult(const Vector& x, const Vector& y, Vector& w) const;

  // IInternalLinearAlgebra interface.

  void mult(const Matrix& a, const UniqueArray<Real>& x, UniqueArray<Real>& r) const;
  void axpy(Real alpha, UniqueArray<Real> const& x, UniqueArray<Real>& r) const;
  void aypx(Real alpha, UniqueArray<Real>& y, UniqueArray<Real> const& x) const;
  void copy(const UniqueArray<Real>& x, UniqueArray<Real>& r) const;
  Real dot(
  Integer local_size, const UniqueArray<Real>& x, const UniqueArray<Real>& y) const;

  void scal(Real alpha, UniqueArray<Real>& x) const;

 private:
  // No member.
};

} // namespace Alien

/*---------------------------------------------------------------------------*/
