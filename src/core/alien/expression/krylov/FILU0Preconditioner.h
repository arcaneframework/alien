/*
 * FILU0Preconditioner.h
 *
 *  Created on: June 15, 2017
 *      Author: gratien
 */

#pragma once

namespace Alien
{


  template<typename MatrixT, typename VectorT>
  class FLUFactorisationAlgo
  : public LUFactorisationAlgo<MatrixT,VectorT>
  {
  public :
    typedef LUFactorisationAlgo<MatrixT,VectorT> BaseType ;
    typedef MatrixT                              MatrixType;
    typedef VectorT                              VectorType;
    typedef typename MatrixType::ProfileType     ProfileType ;
    typedef typename MatrixType::ValueType       ValueType;
    FLUFactorisationAlgo()
    {}

    virtual ~FLUFactorisationAlgo()
    {}

    void setParameter(std::string param, int value) {
      if(param.compare("nb-factor-iter")==0)
        m_nb_factorization_iter = value ;
      else if(param.compare("nb-solver-iter")==0)
        m_nb_solver_iter = value ;
    }

    void setParameter(std::string param, double value) {
      if(param.compare("tol")==0)
        m_tol = value ;
    }

    template<typename AlgebraT>
    void init(AlgebraT& algebra, MatrixT const& matrix)
    {
      BaseType::baseInit(algebra,matrix) ;
      algebra.allocate(AlgebraT::resource(matrix),m_xk) ;

      if(m_nb_factorization_iter==0)
      {
        BaseType::factorize(*this->m_lu_matrix) ;
      }
      else
      {
        auto const& profile = matrix.getProfile() ;
        auto nrows = Alien::numRows(matrix) ;
        int const* kcol = Alien::kcol(profile) ;
        int const* dcol = Alien::dcol(profile) ;
        int const* cols = Alien::cols(profile) ;

        typename BaseType::ValueType const* matrix_values = Alien::dataPtr(matrix) ;
        //auto const& profile = matrix.getProfile() ;
        auto nnz = Alien::nnz(matrix) ;

        for(int iter=0;iter<m_nb_factorization_iter;++iter)
        {
            ValueType* values = Alien::dataPtr(*this->m_lu_matrix) ;

            std::vector<ValueType> guest_values(nnz) ;
            std::copy(values,values+nnz,guest_values.data()) ;
            std::copy(matrix_values,matrix_values+nnz,values) ;

            factorizeInternal(nrows,kcol,dcol,cols,values,guest_values.data()) ;

        }
      }
      this->m_work.clear() ;

      algebra.allocate(AlgebraT::resource(matrix),m_inv_diag) ;
      algebra.assign(m_inv_diag,1.);
      algebra.computeInvDiag(*this->m_lu_matrix,m_inv_diag) ;
    }

    ///////////////////////////////////////////////////////////////////////
    //
    // FACTORIZATION
    //
    void factorizeInternal(std::size_t nrows,
                           int const* kcol,
                           int const* dcol,
                           int const* cols,
                           ValueType* values,
                           ValueType* values0)
    {
      /*
       *
         For i = 1, . . . ,N Do:
            For k = 1, . . . , i - 1 and if (i, k) 2 NZ(A) Do:
                Compute aik := aik/akj
                For j = k + 1, . . . and if (i, j) 2 NZ(A), Do:
                   compute aij := aij - aik.ak,j.
                EndFor
            EndFor
         EndFor
       *
       */

      for(std::size_t irow=1;irow<nrows;++irow)           // i=1->nrow
      {
        _factorizeRow(irow,kcol,dcol,cols,values,values0) ;
      }
    }


    ///////////////////////////////////////////////////////////////////////
    //
    // TRIANGULAR RESOLUTION
    //

    template<typename AlgebraT>
    void solveL(AlgebraT& algebra,
                VectorType const& y,
                VectorType& x,
                VectorType& xk) const
    {
#ifdef DEBUG
        auto const& profile = this->m_lu_matrix->getProfile() ;
        int nrows = Alien::numRows(*this->m_lu_matrix) ;
        int const* kcol = Alien::kcol(profile) ;
        int const* dcol = Alien::dcol(profile) ;
        int const* cols = Alien::cols(profile) ;

        ValueType* values = Alien::dataPtr(*this->m_lu_matrix) ;

        std::copy(x,x+nrows,xk) ;
        for(int irow=0;irow<nrows;++irow)
        {
            ValueType val = y[irow] ;
            for(int k=kcol[irow];k<dcol[irow];++k)
              val -= values[k]*xk[cols[k]] ;
            x[irow] = val ;
        }
#endif
        algebra.copy(x,xk) ;
        algebra.copy(y,x) ;
        algebra.addLMult(-1,*this->m_lu_matrix,xk,x) ;
     }

    template<typename AlgebraT>
    void solveU(AlgebraT& algebra, VectorType const& y, VectorType& x,VectorType& xk) const
    {
#ifdef DEBUG
      auto const& profile = this->m_lu_matrix->getProfile() ;
      int nrows = Alien::numRows(*this->m_lu_matrix) ;
      int const* kcol = Alien::kcol(profile) ;
      int const* dcol = Alien::dcol(profile) ;
      int const* cols = Alien::cols(profile) ;

      ValueType* values = Alien::dataPtr(*this->m_lu_matrix) ;

      std::copy(x,x+nrows,xk) ;
      for(int irow=0;irow<nrows;++irow)
      {
        int dk = dcol[irow] ;
        ValueType val = y[irow] ;
        for(int k=dk+1;k<kcol[irow+1];++k)
        {
          val -= values[k]*xk[cols[k]] ;
        }
        val = inv(values[dk]) * val ;
        x[irow] = val ;
      }
#endif
      algebra.copy(x,xk) ;
      algebra.copy(y,x) ;
      algebra.addUMult(-1.,*this->m_lu_matrix,xk,x) ;
      algebra.pointwiseMult(m_inv_diag,x,x) ;
      //algebra.multInvDiag(*this->m_lu_matrix,x) ;
    }

    template<typename AlgebraT>
    void solve(AlgebraT& algebra, VectorType const& x, VectorType& y) const
    {

      //////////////////////////////////////////////////////////////////////////
      //
      //     L.X1 = Y
      //

      algebra.copy(x,this->m_x) ;
      for(int iter=0;iter<m_nb_solver_iter;++iter)
      {
         solveL(algebra,x,this->m_x,m_xk) ;
      }

      //////////////////////////////////////////////////////////////////////
      //
      // Solve U.X = L-1(Y)
      //
      algebra.copy(this->m_x,y) ;
      for(int iter=0;iter<m_nb_solver_iter;++iter)
      {
         solveU(algebra,this->m_x,y,m_xk) ;
      }
    }

  private :

    void _factorizeRow(std::size_t irow,
                      int const* kcol,
                      int const* dcol,
                      int const* cols,
                      ValueType* values,
                      ValueType* values0)
    {
      /*
       *
            For k = 1, . . . , i - 1 and if (i, k) 2 NZ(A) Do:
                Compute aik := aik/akj
                For j = k + 1, . . . and if (i, j) 2 NZ(A), Do:
                   compute aij := aij - aik.ak,j.
                EndFor
            EndFor
       *
       */

       for(int k=kcol[irow];k<dcol[irow];++k)  // k=1 ->i-1
       {
         int krow = cols[k] ;
         typename BaseType::ValueType aik = values[k] / values0[dcol[krow]] ;
         values[k] = aik ; // aik = aik/akk
         for(int l=kcol[krow];l<kcol[krow+1];++l)
           this->m_work[cols[l]] = l ;
         for(int j=k+1;j<kcol[irow+1];++j)   // j=k+1->n
         {
           int jcol = cols[j] ;
           int kj = this->m_work[jcol];
           if(kj!=-1)
           {
             values[j] -= aik*values0[kj] ;                      // aij = aij - aik*akj
           }
         }
         for(int l=kcol[krow];l<kcol[krow+1];++l)
           this->m_work[cols[l]] = -1 ;
       }
    }

    mutable VectorType        m_xk ;
    mutable VectorType        m_inv_diag ;

  public:
    //!PARAMETERS
    int       m_nb_factorization_iter = 0 ;
    int       m_nb_solver_iter        = 0 ;
    ValueType m_tol                   = 0 ;

  } ;

  template<typename AlgebraT>
  class FILU0Preconditioner
  {
  public:

    typedef AlgebraT                        AlgebraType ;
    typedef typename AlgebraType::Matrix    MatrixType;
    typedef typename AlgebraType::Vector    VectorType;
    typedef typename MatrixType::ValueType  ValueType;


    typedef FLUFactorisationAlgo<MatrixType,VectorType> AlgoType ;

    FILU0Preconditioner(AlgebraType& algebra,
                        MatrixType const& matrix,
                        ITraceMng* trace_mng=nullptr)
    : m_algebra(algebra)
    , m_matrix(matrix)
    , m_trace_mng(trace_mng)
    {}

    virtual ~FILU0Preconditioner()
    {}

    void setParameter(std::string param, int value)
    {
      m_algo.setParameter(param,value) ;
    }

    void setParameter(std::string param, double value)
    {
      m_algo.setParameter(param,value) ;
    }

    void init()
    {
      m_algo.init(m_algebra, m_matrix) ;
      if(m_trace_mng)
      {
        m_trace_mng->info()<<"FILU Preconditioner :";
        m_trace_mng->info()<<"     Nb Factor iter :"<<m_algo.m_nb_factorization_iter;
        m_trace_mng->info()<<"     Nb Solver iter :"<<m_algo.m_nb_solver_iter;
        m_trace_mng->info()<<"     Tolerance      :"<<m_algo.m_tol;
      }
    }

    void solve(VectorType const& y, VectorType& x) const
    {
      m_algo.solve(m_algebra,y,x) ;
    }


    void solve(AlgebraType& algebra,VectorType const& y, VectorType& x) const
    {
      m_algo.solve(algebra,y,x) ;
    }

  private :

    AlgebraType&                  m_algebra ;
    MatrixType const&             m_matrix;
    AlgoType                      m_algo ;

    ITraceMng*                    m_trace_mng = nullptr ;
    bool                          m_verbose   = false ;

  };

}
