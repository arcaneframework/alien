/*
 * ILU0Preconditioner.h
 *
 *  Created on: Sep 20, 2010
 *      Author: gratienj
 */

#pragma once



namespace Alien
{
  template<typename MatrixT>
  std::size_t numRows(MatrixT const& matrix)
  {
    return matrix.getProfile().getNRows() ;
  }


  template<typename MatrixT>
  std::size_t nnz(MatrixT const& matrix)
  {
    return matrix.getProfile().getNnz() ;
  }

  template<typename ProfileT>
  int const* kcol(ProfileT const& profile)
  {
    return profile.kcol() ;
  }

  template<typename ProfileT>
  int const* dcol(ProfileT const& profile)
  {
    return profile.dcol() ;
  }


  template<typename ProfileT>
  int const* cols(ProfileT const& profile)
  {
    return profile.cols() ;
  }


  template<typename MatrixT>
  typename  MatrixT::ValueType const* dataPtr(MatrixT const& matrix)
  {
    return matrix.getAddressData() ;
  }

  template<typename MatrixT>
  typename  MatrixT::ValueType* dataPtr(MatrixT& matrix)
  {
    return matrix.getAddressData() ;
  }

  template<typename MatrixT, typename VectorT>
  class LUFactorisationAlgo
  {
  public :
    typedef MatrixT                          MatrixType;
    typedef VectorT                          VectorType;
    typedef typename MatrixType::ProfileType ProfileType ;
    typedef typename MatrixType::ValueType   ValueType;
    LUFactorisationAlgo()
    {}

    virtual ~LUFactorisationAlgo()
    {}

    template<typename AlgebraT>
    void baseInit(AlgebraT& algebra, MatrixT const& matrix)
    {
      m_lu_matrix.reset(matrix.cloneTo(nullptr));
      m_profile = &m_lu_matrix->getProfile() ;
      m_alloc_size = Alien::numRows(*m_lu_matrix) ;
      m_work.resize(m_alloc_size) ;
      m_work.assign(m_work.size(),-1) ;
      algebra.allocate(AlgebraT::resource(matrix),m_x) ;
    }

    template<typename AlgebraT>
    void init(AlgebraT& algebra,MatrixT const& matrix)
    {
      baseInit(algebra,matrix) ;
      factorize(*m_lu_matrix) ;
      m_work.clear() ;
    }

    void factorize(MatrixT& matrix)
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
      auto nrows = Alien::numRows(matrix) ;
      int const* kcol = Alien::kcol(profile) ;
      int const* dcol = Alien::dcol(profile) ;
      int const* cols = Alien::cols(profile) ;
      ValueType* values = Alien::dataPtr(matrix) ;
      for(std::size_t irow=1;irow<nrows;++irow)           // i=1->nrow
      {
         for(int k=kcol[irow];k<dcol[irow];++k)  // k=1 ->i-1
         {
           int krow = cols[k] ;
           ValueType aik = values[k] / values[dcol[krow]] ; // aik = aik/akk
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


    void solveL(ValueType const* y, ValueType* x) const
    {
        auto const& profile = m_lu_matrix->getProfile() ;
        auto nrows = Alien::numRows(*m_lu_matrix) ;
        int const* kcol = Alien::kcol(profile) ;
        int const* dcol = Alien::dcol(profile) ;
        int const* cols = Alien::cols(profile) ;
        ValueType* values = Alien::dataPtr(*m_lu_matrix) ;
        for(std::size_t irow=0;irow<nrows;++irow)
        {
          ValueType val = y[irow] ;
          for(int k=kcol[irow];k<dcol[irow];++k)
            val -= values[k]*x[cols[k]] ;
          x[irow] = val ;
        }
    }


    void solveU(ValueType const* y, ValueType* x) const
    {
        auto const& profile = m_lu_matrix->getProfile() ;
        auto nrows = Alien::numRows(*m_lu_matrix) ;
        int const* kcol = Alien::kcol(profile) ;
        int const* dcol = Alien::dcol(profile) ;
        int const* cols = Alien::cols(profile) ;
        ValueType* values = Alien::dataPtr(*m_lu_matrix) ;
        for(int irow = nrows-1;irow>-1;--irow)
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

    template<typename AlgebraT>
    void solve(AlgebraT& algebra, VectorType const& y, VectorType& x) const
    {

      //////////////////////////////////////////////////////////////////////////
      //
      //     L.X1 = Y
      //
      solveL(y.data(),m_x.data()) ;

      //////////////////////////////////////////////////////////////////////////
      //
      //     U.X = X1
      //
      solveU(m_x.data(),x.data()) ;
    }

    const MatrixType &getLUMatrix() const
    {
      return *m_lu_matrix;
    }

  protected :
    std::unique_ptr<MatrixType>   m_lu_matrix ;
    ProfileType const*            m_profile                     = nullptr;
    mutable VectorType            m_x ;

    std::vector<int>              m_work ;
    std::size_t                   m_alloc_size                  = 0 ;

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

    typedef LUFactorisationAlgo<MatrixType,VectorType> AlgoType ;

    ILU0Preconditioner(AlgebraType& algebra, MatrixType const& matrix, ITraceMng* trace_mng=nullptr)
    : m_algebra(algebra)
    , m_matrix(matrix)
    , m_trace_mng(trace_mng)
    {

    }

    virtual ~ILU0Preconditioner()
    {};

    void init()
    {
      m_algo.init(m_algebra,m_matrix) ;
    }

    void solve(VectorType const& y, VectorType& x) const
    {
      m_algo.solve(m_algebra,y,x) ;
    }


    void solve(AlgebraType& algebra,VectorType const& y, VectorType& x) const
    {
      m_algo.solve(algebra,y,x) ;
    }


    void update(){
      // update value from m_matrix
    }


  private :
    AlgebraType&                  m_algebra ;
    MatrixType const&             m_matrix;
    AlgoType                      m_algo ;

    ITraceMng*                    m_trace_mng = nullptr ;

  };

}
