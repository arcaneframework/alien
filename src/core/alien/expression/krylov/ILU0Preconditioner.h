/*
 * ILU0Preconditioner.h
 *
 *  Created on: Sep 20, 2010
 *      Author: gratienj
 */

#pragma once



namespace Alien
{
  template<typename MatrixT, typename VectorT>
  class LUFactorisationAlgp
  {
  public :
    void init(MatrixT const& matrix)
    {
      m_lu_matrix.reset(new MatrixType());
      m_lu_matrix->clone(matrix);
      m_profile = &m_lu_matrix->getProfile() ;
      m_max_external_size = 0 ;
      m_alloc_size = Alien::numRows(*m_lu_matrix) ;
      m_x.resize(m_alloc_size) ;
      m_work.resize(m_alloc_size) ;
      m_work.assign(m_work.size(),-1) ;
      factorizeInternal() ;
      m_work.clear() ;

    }

    void factorize()
    {
      /*
       *
         For i = 1, . . . ,N Do:
            For k = 1, . . . , i - 1 and if (i, k) 2 NZ(A) Do:
                Compute aik := aik/akk
                For j = k + 1, . . . and if (i, j) 2 NZ(A), Do:
                   compute aij := aij - aik.ak,j.
                EndFor
            EndFor
         EndFor
       *
       */

      auto const& profile = matrix.getProfile() ;
      int nrows = Alien::numRows(matrix) ;
      int const* kcol = Alien::kcol(profile) ;
      int const* dcol = Alien::dcol(profile) ;
      int const* cols = Alien::cols(profile) ;
      ValueType* values = Alien::dataPtr(matrix) ;
      for(int irow=1;irow<nrows;++irow)           // i=1->nrow
      {
         for(int k=kcol[irow];k<dcol[irow];++k)  // k=1 ->i-1
         {
           int krow = cols[k] ;
           ValueType aik = values[k] * inv(values[dcol[krow]]) ; // aik = aik/akk
           values[k] = aik ;
           for(int l=kcol[krow];l<kcol[krow+1];++l)
             m_work[cols[l]] = l ;
           for(int j=k+1;j<kcol[irow+1];++j)   // j=k+1->n
           {
             int jcol = cols[j] ;
             int kj = m_work[jcol];
             if(kj!=-1)
             {
               values[j] -= aik*values[kj] ;                      // aij = aij - aik*akj
             }
           }
           for(int l=kcol[krow];l<kcol[krow+1];++l)
             m_work[cols[l]] = -1 ;
         }
      }
    }

    virtual void factorizeInternal(int domain_id)
    {

      int const* kcol = m_blk_data.m_kcol ;
      int const* dcol = m_blk_data.m_dcol ;
      int const* cols = m_blk_data.m_cols ;
      ValueType* values = m_blk_data.m_values  ;
      int int_size = m_blk_data.m_nrows ;
      for(int irow=1;irow<int_size;++irow)           // i=1->nrow
      {
         for(int k=kcol[irow];k<dcol[irow];++k)  // k=1 ->i-1
         {
           int krow = cols[k] ;
           ValueType aik = values[k] * inv(values[dcol[krow]]) ; // aik = aik/akk
           values[k] = aik ;
           for(int l=kcol[krow];l<kcol[krow+1];++l)
             m_work[cols[l]] = l ;
           for(int j=k+1;j<kcol[irow+1];++j)   // j=k+1->n
           {
             int jcol = cols[j] ;
             int kj = m_work[jcol];
             if(kj!=-1)
             {
               values[j] -= aik*values[kj] ;                      // aij = aij - aik*akj
             }
           }
           for(int l=kcol[krow];l<kcol[krow+1];++l)
             m_work[cols[l]] = -1 ;
         }
      }
    }
    void initWork(int krow,CSRData const& data)
    {
      for(int l=data.m_kcol[krow];l<data.m_kcol[krow+1];++l)
        m_work[data.m_domain_offset+data.m_cols[l]] = l ;
      if(krow>=data.m_internal_size)
        for(int l=data.m_ext_dcol[krow-data.m_internal_size];l<data.m_ext_kcol[krow+1-data.m_internal_size];++l)
        m_work[data.m_external_node_gid[data.m_ext_cols[l]]] = -2-l ;
    }
    void resetWork(int krow,CSRData const& data)
    {
      for(int l=data.m_kcol[krow];l<data.m_kcol[krow+1];++l)
        m_work[data.m_domain_offset+data.m_cols[l]] = -1 ;
      if(krow>=data.m_internal_size)
        for(int l=data.m_ext_dcol[krow-data.m_internal_size];l<data.m_ext_kcol[krow+1-data.m_internal_size];++l)
        m_work[data.m_external_node_gid[data.m_ext_cols[l]]] = -1 ;
    }


    void solveL(MatrixType& matrix,ValueType const* y, ValueType* x) const
    {

        auto const& profile = matrix.getProfile() ;
        int nrows = Alien::numRows(matrix) ;
        int const* kcol = Alien::kcol(profile) ;
        int const* dcol = Alien::dcol(profile) ;
        int const* cols = Alien::cols(profile) ;
        ValueType* values = Alien::dataPtr(matrix) ;
        for(int irow=0;irow<nrows;++irow)
        {
          ValueType val = y[irow] ;
          for(int k=kcol[irow];k<dcol[irow];++k)
            val -= values[k]*x[cols[k]] ;
          x[irow] = val ;
        }
    }

    virtual void solveLInternal(ValueType const* y, ValueType* x) const
     {

          int const* kcol = m_blk_data.m_kcol ;
          int const* dcol = m_blk_data.m_dcol ;
          int const* cols = m_blk_data.m_cols ;
          ValueType* values = m_blk_data.m_values  ;
          int int_size = m_blk_data.m_nrows ;
         for(int irow=0;irow<int_size;++irow)
         {
           ValueType val = y[irow] ;
           for(int k=kcol[irow];k<dcol[irow];++k)
             val -= values[k]*x[cols[k]] ;
           x[irow] = val ;
         }
     }



    void solveU(MatrixType& matrix,ValueType const* y, ValueType* x) const
    {

        auto const& profile = matrix.getProfile() ;
        int nrows = Alien::numRows(matrix) ;
        int const* kcol = Alien::kcol(profile) ;
        int const* dcol = Alien::dcol(profile) ;
        int const* cols = Alien::cols(profile) ;
        ValueType* values = Alien::dataPtr(matrix) ;
        for(int irow=nrows-1;irow>-1;--irow)
        {
          int dk = dcol[irow] ;
          ValueType val = y[irow] ;
          for(int k=dk+1;k<kcol[irow+1];++k)
          {
            val -= values[k]*x[cols[k]] ;
          }
          x[irow] = val/values[dk] ;
        }
    }


    virtual void solveUInternal(ValueType const* y, ValueType* x) const
    {

        int const* kcol = m_blk_data.m_kcol ;
        int const* dcol = m_blk_data.m_dcol ;
        int const* cols = m_blk_data.m_cols ;
        ValueType* values = m_blk_data.m_values  ;
        int int_size = m_blk_data.m_nrows ;
        for(int irow=int_size-1;irow>-1;--irow)
        {
          int dk = dcol[irow] ;
          ValueType val = y[irow] ;
          for(int k=dk+1;k<kcol[irow+1];++k)
          {
            val -= values[k]*x[cols[k]] ;
          }
          x[irow] = inv(values[dk]) * val ;
        }
    }
    const MatrixType &getLUMatrix() const
    {
      return *m_lu_matrix;
    }

  private :
    std::unique_ptr<MatrixType>   m_lu_matrix ;
    ProfileType const*            m_profile                     = nullptr;
    std::vector<int>              m_work ;
    CSRData                       m_blk_data ;
    std::size_t                   m_alloc_size                  = 0 ;

    mutable VectorType            m_x ;

    int m_max_external_size                                     = 0 ;
  };

  template<typename AlgebraT>
  class ILU0Preconditioner
  {
  public:
    typedef AlgebraT                         AlgebraType ;
    typedef typename AlgebraType::Matrix     MatrixType;
    typedef typename AlgebraType::Vector     VectorType;
    typedef typename MatrixType::ProfileType ProfileType ;
    typedef typename MatrixType::ValueType   ValueType;
    struct CSRData
    {
      int              m_nrows ;
      const int*       m_cols ;
      const int*       m_kcol ;
      const int*       m_dcol ;
      MatrixType*      m_values ;
    };


    ILU0Preconditioner(MatrixType& matrix, ITraceMng* trace_mng=nullptr)
    : m_matrix(matrix)
    , m_trace_mng(trace_mng)
    {

    }

    virtual ~ILU0Preconditioner()
    {};


    //! operator preparation
    void baseInit()
    {
      m_lu_algo.init(m_matrix) ;
    }

    void init()
    {
      baseInit() ;
    }




    void solve(VectorType const& y, VectorType& x) const
    {

      //////////////////////////////////////////////////////////////////////////
      //
      //     L.X1 = Y
      //
      solveLInternal(y.data(),m_x.data()) ;

      solveUInternal(m_x.data(),x.data()) ;
    }


    void update(){
      // update value from m_matrix
    }




  protected :
    MatrixType const&             m_matrix;


    ITraceMng*                    m_trace_mng                   = nullptr ;

  };

}
