/*
 * SYCLVector.h
 *
 *  Created on: Nov 20, 2021
 *      Author: gratienj
 */

#pragma once


#include <alien/core/impl/IVectorImpl.h>
#include <alien/data/ISpace.h>
#include <alien/kernels/sycl/SYCLBackEnd.h>
#include <alien/kernels/sycl/SYCLPrecomp.h>
#include <iostream>

/*---------------------------------------------------------------------------*/

namespace Alien
{

  namespace SYCLInternal
  {

    template <typename ValueT>
    class VectorInternal;

  }

template <typename ValueT>
class ALIEN_EXPORT SYCLVector : public IVectorImpl
{
 public:
  typedef ValueT ValueType;

  typedef SYCLInternal::VectorInternal<ValueType> VectorInternal;

  //! Constructeur sans association un MultiImpl
  SYCLVector()
  : IVectorImpl(nullptr, AlgebraTraits<BackEnd::tag::sycl>::name())
  {}

  //! Constructeur avec association à un MultiImpl
  SYCLVector(const MultiVectorImpl* multi_impl)
  : IVectorImpl(multi_impl, AlgebraTraits<BackEnd::tag::sycl>::name())
  {}

  virtual ~SYCLVector () ;

  VectorInternal* internal() {
    return m_internal ;
  }

  VectorInternal const* internal() const {
    return m_internal ;
  }

  Integer getAllocSize() const
  {
    return m_local_size;
  }

  void allocate() ;

  void resize(Integer alloc_size) const ;


  void clear() ;

  void init(const VectorDistribution& dist, const bool need_allocate)
  {
    alien_debug([&] { cout() << "Initializing SYCLVector " << this; });
    if(this->m_multi_impl)
    {
      m_local_size = this->scalarizedLocalSize();
    }
    else
    {
      // Not associated vector
      m_own_distribution = dist ;
      m_local_size = m_own_distribution.localSize() ;
    }
    if (need_allocate) {
      allocate() ;
    }
  }

  const VectorDistribution& distribution() const
  {
    if(this->m_multi_impl)
      return IVectorImpl::distribution() ;
    else
      return m_own_distribution ;
  }

  Arccore::Integer scalarizedLocalSize() const
  {
    if(this->m_multi_impl)
      return IVectorImpl::scalarizedLocalSize() ;
    else
      return m_own_distribution.localSize() ;
  }

  Arccore::Integer scalarizedGlobalSize() const
  {
    if(this->m_multi_impl)
      return IVectorImpl::scalarizedGlobalSize() ;
    else
      return m_own_distribution.globalSize() ;
  }

  Arccore::Integer scalarizedOffset() const
  {
    if(this->m_multi_impl)
      return IVectorImpl::scalarizedOffset() ;
    else
      return m_own_distribution.offset() ;
  }

  template <typename LambdaT>
  void apply(LambdaT const& lambda)
  {
    for(std::size_t i=0;i<m_local_size;++i)
    {
      m_h_values[i] = lambda(i) ;
    }
    setValuesFromHost() ;
  }

  void setValues(std::size_t size, ValueType const* ptr ) ;

  void setValuesFromHost() ;

  void copyValuesTo(std::size_t size, ValueType* ptr ) const ;


  // FIXME: not implemented !
  template <typename E>
  SYCLVector& operator=(E const& expr);

 private:
  mutable VectorInternal*        m_internal = nullptr;
  mutable std::vector<ValueType> m_h_values ;
  std::size_t                    m_local_size = 0;
  VectorDistribution             m_own_distribution ;
};

//extern template class SYCLVector<Real>;
}

