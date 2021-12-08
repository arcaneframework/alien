/*
 * MCILU0Preconditioner.h
 *
 *  Created on: Sep 20, 2010
 *      Author: gratienj
 */

#pragma once

namespace Alien
{
  template<typename AlgebraT>
  class DiagPreconditioner
  {
  public:
    typedef AlgebraT                        AlgebraType ;
    typedef typename AlgebraType::Matrix    MatrixType;
    typedef typename AlgebraType::Vector    VectorType;
    typedef typename MatrixType::ValueType  ValueType;

    DiagPreconditioner(MatrixType const& matrix)
    : m_matrix(matrix)
    {}

    virtual ~DiagPreconditioner()
    {
    };

    //! operator preparation
    void init(){
    }

    void update(){
      // update value from m_matrix
    }

    template<typename AlgebraType>
    void solve(AlgebraType & algebra,
               VectorType const& x,
               VectorType& y) const
    {
      algebra.copy(x,y) ;
    }

  private :
    MatrixType const&           m_matrix;
  };

}

