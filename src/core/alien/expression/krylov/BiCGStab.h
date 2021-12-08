/*
 * bicgs.h
 *
 *  Created on: Dec 1, 2021
 *      Author: gratienj
 */

#pragma once

#include <ostream>
#include <vector>

namespace Alien
{

  template<typename AlgebraT>
  class BiCGStab
  {
    public:
      typedef AlgebraT                        AlgebraType;
      typedef typename AlgebraType::Matrix    MatrixType;
      typedef typename AlgebraType::Vector    VectorType;
      typedef typename MatrixType::ValueType  ValueType;

      class Iteration
      {
        public:

          Iteration (AlgebraType& algebra,
                     VectorType const& b,
                     ValueType tol,
                     int max_iter,
                     ITraceMng* trace_mng = nullptr)
              : m_algebra (algebra)
              , m_max_iteration (max_iter)
              , m_tol (tol)
              , m_iter (0)
              , m_trace_mng(trace_mng)
          {
            m_nrm2_b = m_algebra.dot (b, b);
            if(m_trace_mng)
              m_trace_mng->info()<<"STOP CRITERIA NORME B = "<<m_nrm2_b ;
            m_criteria_value = m_tol * m_tol * m_nrm2_b;
            m_sqrt_nrm2_b = std::sqrt (m_nrm2_b);
            m_value = m_criteria_value + 1;
            if(m_nrm2_b==0)
              m_status = true ;
            else
              m_status = false;
          }

          virtual ~Iteration ()
          {}

          bool nullRhs() const {
            return m_nrm2_b==0. ;
          }

          bool first () const
          {
            return m_iter == 0;
          }

          bool stop (VectorType const& r)
          {
            if (m_iter >= m_max_iteration)
              return true;
            m_value = m_algebra.dot (r, r);
            m_status = m_value < m_criteria_value;
            return m_status;
          }

          void operator++ ()
          {
            if(m_trace_mng)
              m_trace_mng->info()<<"ITERATION ("<<m_iter<<","<<m_value<<")";
            ++m_iter;
          }

          ValueType getValue () const
          {
            if(m_sqrt_nrm2_b==0)
              return 0. ;
            else
              return std::sqrt (m_value) / m_sqrt_nrm2_b;
          }

          int operator() () const
          {
            return m_iter;
          }

          bool getStatus () const
          {
            return m_status;
          }

        private:
          AlgebraType& m_algebra;
          int          m_max_iteration  = 0;
          ValueType    m_tol            = 0.;
          int          m_iter           = 0;
          ValueType    m_value          = 0.;
          ValueType    m_criteria_value = 0.;
          ValueType    m_value_init     = 0.;
          ValueType    m_nrm2_b         = 0.;
          ValueType    m_sqrt_nrm2_b    = 0.;
          bool         m_status         = false;
          ITraceMng*   m_trace_mng      = nullptr;
      };

      BiCGStab (AlgebraType& algebra, ITraceMng* trace_mng = nullptr)
      : m_algebra (algebra)
      , m_trace_mng(trace_mng)
      {}

      virtual ~BiCGStab ()
      {}

      void setOutputLevel(int level) {
        m_output_level = level ;
      }

      template<typename PrecondT, typename iterT>
      int solve (PrecondT& precond, iterT& iter, MatrixType const& A,
                 VectorType const& b, VectorType& x)
      {
        if(iter.nullRhs())
           return 0 ;
        ValueType rho (0), rho1 (0), alpha (0), beta (0), gamma (0), omega (0);
        VectorType p, phat, s, shat, t, v, r, r0;

        m_algebra.allocate (AlgebraType::resource (A), p, phat, s, shat, t, v, r, r0) ;

        // SEQ0
        //  r = b - A * x;
        m_algebra.copy (b, r);
        m_algebra.mult (A, x, r0);
        m_algebra.axpy (-1., r0, r);

        // rtilde = r
        m_algebra.copy (r, r0);
        m_algebra.copy (r, p);
        rho1 = m_algebra.dot (r, r0);
        m_trace_mng->info()<<"RHO1"<<rho1;

        /*
           phat = solve(M, p);
           v = A * phat;
           gamma = dot(r0, v);
           alpha = rho_1 / gamma;
           s = r - alpha * v;
         */
        // SEQ1
        m_algebra.exec (precond, p, phat);
        m_algebra.mult (A, phat, v);
        alpha = m_algebra.dot (v, r0);
        if (alpha == 0)
            throw typename AlgebraType::NullValueException ("alpha");
        alpha = rho1/alpha ;

        m_algebra.copy (r, s);
        m_algebra.axpy (-alpha, v, s);

        if (iter.stop(s))
        {
           ++iter;
           m_algebra.axpy (alpha, phat, x);
           m_algebra.free (p, phat, s, shat, t, v, r, r0);
           return 0;
        }


        // SEQ 2
        m_algebra.exec (precond, s, shat);
        m_algebra.mult (A, shat, t);
        omega = m_algebra.dot (t, s);
        beta = m_algebra.dot (t, t);
        if (beta == 0)
        {
             if (iter.stop (r))
             {
               ++iter;
               m_algebra.axpy (alpha, phat, x);
               m_algebra.free (p, phat, s, shat, t, v, r, r0);
               return 0;
             }
             else
               throw typename AlgebraType::NullValueException ("beta");
        }

        omega = omega/beta ;

        // SEQ 3
        m_algebra.axpy (omega, shat, x);
        m_algebra.axpy (alpha, phat, x);
        m_algebra.copy (s, r);
        m_algebra.axpy (-omega, t, r);

        rho = rho1;
        ++iter;

         //SEQ4
         /*
           beta = (rho_1 / rho_2) * (alpha / omega);
           p = r + beta * (p - omega * v);
         */
        rho1 = m_algebra.dot (r, r0);
        beta = (rho1/rho)*(alpha/omega) ;
        m_algebra.axpy (-omega, v, p);
        m_algebra.scal (beta, p);
        m_algebra.axpy (-1.,r, p);

        while (!iter.stop (r))
        {

          if (m_output_level > 0 && m_trace_mng)
            m_trace_mng->info()<<"Iteration : "<<iter ()<<" criteria value="<<iter.getValue() ;
          //SEQ4
          rho1 = m_algebra.dot (r, r0);
          beta = (rho1/rho)*(alpha/omega);
          m_algebra.axpy (-omega, v, p);
          m_algebra.scal (beta, p);
          m_algebra.axpy (-1.,r, p);
          if(rho==0)
            throw typename AlgebraType::NullValueException ("rho");

          //m_algebra.process (seq1);
          m_algebra.exec (precond, p, phat);
          m_algebra.mult (A, phat, v);
          alpha = m_algebra.dot (v, r0);
          if(alpha==0)
            throw typename AlgebraType::NullValueException ("alpha");
          else
            alpha = rho1/alpha ;

          m_algebra.copy (r, s);
          m_algebra.axpy (-alpha, v, s);

          if (iter.stop(s))
          {
            m_algebra.axpy (alpha, phat, x);
            m_algebra.free (p, phat, s, shat, t, v, r, r0);
            return 0;
          }

          //m_algebra.process (seq2);
          m_algebra.exec (precond, s, shat);
          m_algebra.mult (A, shat, t);
          omega = m_algebra.dot (t, s);
          beta = m_algebra.dot (t, t);
          if (beta == 0)
          {
            if (iter.stop (s))
            {
              m_algebra.axpy (alpha, phat, x);
              m_algebra.free (p, phat, s, shat, t, v, r, r0);
              return 0;
            }
            throw typename AlgebraType::NullValueException ("beta");
          }
          else
            omega = omega/beta ;

          //m_algebra.process (seq3);
          m_algebra.axpy (omega, shat, x);
          m_algebra.axpy (alpha, phat, x);
          m_algebra.copy (s, r);
          m_algebra.axpy (-omega, t, r);

          rho = rho1;

          ++iter;
        }

        m_algebra.free (p, phat, s, shat, t, v, r, r0);

        return 0;
      }


    private:
      AlgebraType& m_algebra;
      ITraceMng*   m_trace_mng    = nullptr ;
      int          m_output_level = 0 ;

  };
}
