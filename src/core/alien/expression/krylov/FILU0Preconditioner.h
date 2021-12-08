/*
 * FILU0Preconditioner.h
 *
 *  Created on: June 15, 2017
 *      Author: gratien
 */

#pragma once

namespace Alien
{


  template<typename AlgebraT>
  class FILU0Preconditioner
  : public ILU0Preconditioner<AlgebraT>
  {
  public:

    typedef AlgebraT                        AlgebraType ;
    typedef typename AlgebraType::Matrix    MatrixType;
    typedef typename AlgebraType::Vector    VectorType;
    typedef typename MatrixType::ValueType  ValueType;

    typedef ILU0Preconditioner<AlgebraT>    BaseType ;

    class SolveLU : public BaseType::SolveLU
    {
    public :
      typedef typename BaseType::SolveLU ParentType ;

      SolveLU(FILU0Preconditioner* precond, ITraceMng* trace_mng=nullptr)
      : ParentType(precond,trace_mng)
      , m_precond(precond)
      {}


      virtual ~SolveLU(){}



    private:
      FILU0Preconditioner const* m_precond = nullptr;
    };

    FILU0Preconditioner(MatrixT& matrix,ITraceMng* trace_mng=nullptr)
    : BaseType(matrix,trace_mng)
    {}

    virtual ~FILU0Preconditioner()
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

    void init()
    {
      BaseType::baseInit() ;


      this->m_alloc_size = Alien::numRows(*this->m_lu_matrix) ;

      this->m_x.resize(this->m_alloc_size) ;
      m_xk.resize(this->m_alloc_size) ;
      this->m_work.resize(this->m_alloc_size) ;
      this->m_work.assign(this->m_work.size(),-1) ;
      if(m_nb_factorization_iter==0)
      {
          ValueType* values = this->m_blk_data.m_values  ;
          factorizeInternal(values,values) ;
      }
      else
      {
        for(int iter=0;iter<m_nb_factorization_iter;++iter)
        {
            ValueType* values = this->m_blk_data.m_values  ;

            auto const& matrix = this->m_matrix ;
            typename BaseType::ValueType const* matrix_values = Alien::dataPtr(matrix) ;
            //auto const& profile = matrix.getProfile() ;
            int nnz = Alien::nnz(matrix) ;

            std::vector<ValueType> guest_values(nnz) ;
            std::copy(values,values+nnz,guest_values.data()) ;
            std::copy(matrix_values,matrix_values+nnz,values) ;

            factorizeInternal(values,guest_values.data()) ;

        }
      }
      this->m_work.clear() ;

    }
    ///////////////////////////////////////////////////////////////////////
    //
    // FACTORIZATION
    //
    void factorizeInternal(ValueType* values,
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
      int const* kcol = this->m_blk_data.m_kcol ;
      int const* dcol = this->m_blk_data.m_dcol ;
      int const* cols = this->m_blk_data.m_cols ;
      int nrows = this->m_blk_data.m_nrows ;

      for(int irow=1;irow<nrows;++irow)           // i=1->nrow
      {
        _factorizeRow(irow,kcol,dcol,cols,values,values0) ;
      }
    }


    ///////////////////////////////////////////////////////////////////////
    //
    // TRIANGULAR RESOLUTION
    //
    void solveLInternal(ValueType const* y, ValueType* x,ValueType* xk) const
    {

        CSRData const& blk_data = this->m_blk_data ;
        int internal_size = blk_data.m_nrows ;
        int const* kcol = blk_data.m_kcol ;
        int const* dcol = blk_data.m_dcol ;
        int const* cols = blk_data.m_cols ;
        ValueType* values = blk_data.m_values  ;
        std::copy(x,x+domain_size,xk) ;
        for(int irow=0;irow<internal_size;++irow)
        {
            ValueType val = y[irow] ;
            for(int k=kcol[irow];k<dcol[irow];++k)
              val -= values[k]*xk[cols[k]] ;
            x[irow] = val ;
        }
        solveLInterface(y,x,xk) ;
     }

    void solveUInternal(ValueType const* y, ValueType* x,ValueType* xk) const
    {
        using namespace Alien::MatrixVector::Expr ;
        CSRData const& blk_data = this->m_blk_data ;
        int internal_size  = blk_data.m_nrows ;
        int interface_size = blk_data.m_interface_size ;
        int domain_size = internal_size+interface_size ;

        int const* kcol = blk_data.m_kcol ;
        int const* dcol = blk_data.m_dcol ;
        int const* cols = blk_data.m_cols ;
        ValueType* values = blk_data.m_values  ;
        std::copy(x,x+domain_size,xk) ;
        solveUInterface(domain_id,y,x,xk) ;
        //for(int irow=int_size-1;irow>-1;--irow)
        for(int irow=0;irow<internal_size;++irow)
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

    }




  private :

    void _factorizeRow(int irow,
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
         typename BaseType::ValueType aik = values[k] * inv(values0[dcol[krow]]) ;
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


    //!PARAMETERS
    int       m_nb_factorization_iter = 0 ;
    int       m_nb_solver_iter        = 0 ;
    ValueType m_tol                   = 0 ;

    mutable VectorType        m_xk ;
    std::unique_ptr<SolveLU>  m_solverLU ;


    bool      m_verbose               = false ;

  };

}
