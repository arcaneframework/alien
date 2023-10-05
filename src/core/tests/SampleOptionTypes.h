#ifndef HTS_OPTION_TYPES
#define HTS_OPTION_TYPES

struct SampleOptionTypes
{
  enum eSolver
  {
    BCGS,
    CG,
    GMRES,
    LU,
    QR
  };

  enum ePreconditioner
  {
    NonePC,
    DiagPC,
    PolyPC,
    ChebyshevPC,
    ILU0PC,
  };

  enum eStopCriteria
  {
    R,
    RB,
    RR0,
    PR,
    PRB,
    PRR0,
    R2,
    RB2,
    RR02,
    PR2,
    PRB2,
    PRR02
  };

  enum eOpt
  {
    EOpt0,
    EOpt1
  } ;
};

#endif /* HTS_OPTION_TYPES */
