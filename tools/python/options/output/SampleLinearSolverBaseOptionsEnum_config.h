
/*
 * BaseOption.h Generated file
 */

#pragma once

#include <cxxopts.hpp>

namespace Alien {

class SampleLinearSolverBaseOptionsEnum
{
public:

  // ENUM TOOLS
  //! solver option accessor
  static SampleOptionTypes::eSolver SolverStringToEnum(std::string const& key)
  {
      if(key.compare("GMRES")==0)
        return MTLOptionTypes::GMRES ;
      if(key.compare("BiCGStab")==0)
        return SampleOptionTypes::BiCGStab ;
      if(key.compare("CG")==0)
        return SampleOptionTypes::CG ;
      if(key.compare("QR")==0)
        return SampleOptionTypes::QR ;
      if(key.compare("LU")==0)
        return SampleOptionTypes::LU ;
      return SampleOptionTypes::eSolver(0) ;
  }

  static std::string SolverEnumToString(SampleOptionTypes::eSolver value)
  {
      switch(value)
      {
      case MTLOptionTypes::GMRES:
        return std::string("GMRES") ;
      case SampleOptionTypes::BiCGStab:
        return std::string("BiCGStab") ;
      case SampleOptionTypes::CG:
        return std::string("CG") ;
      case SampleOptionTypes::QR:
        return std::string("QR") ;
      case SampleOptionTypes::LU:
        return std::string("LU") ;
      default:
        return std::string("Undefined") ;
      }
  }

  //! preconditioner option accessor
  static SampleOptionTypes::ePreconditioner PreconditionerStringToEnum(std::string const& key)
  {
      if(key.compare("None")==0)
        return SampleOptionTypes::NonePC ;
      if(key.compare("Diag")==0)
        return SampleOptionTypes::DiagPC ;
      if(key.compare("ILU0")==0)
        return SampleOptionTypes::ILU0PC ;
      if(key.compare("ILUT")==0)
        return SampleOptionTypes::ILUTPC ;
      if(key.compare("SSOR")==0)
        return SampleOptionTypes::SSORPC ;
      return SampleOptionTypes::ePreconditioner(0) ;
  }

  static std::string PreconditionerEnumToString(SampleOptionTypes::ePreconditioner value)
  {
      switch(value)
      {
      case SampleOptionTypes::NonePC:
        return std::string("None") ;
      case SampleOptionTypes::DiagPC:
        return std::string("Diag") ;
      case SampleOptionTypes::ILU0PC:
        return std::string("ILU0") ;
      case SampleOptionTypes::ILUTPC:
        return std::string("ILUT") ;
      case SampleOptionTypes::SSORPC:
        return std::string("SSOR") ;
      default:
        return std::string("Undefined") ;
      }
  }


    //! eopt option accessor
    static SampleOptionTypes::eOpt EoptStringToEnum(std::string const& key)
    {
      if(key.compare("EOpt0")==0)
        return SampleOptionTypes::EOpt0 ;
      if(key.compare("EOpt1")==0)
        return SampleOptionTypes::EOpt1 ;
      return SampleOptionTypes::eOpt(0) ;
    }

    static std::string EoptEnumToString(SampleOptionTypes::eOpt value)
    {
      switch(value)
      {
      case SampleOptionTypes::EOpt0:
        return std::string("EOpt0") ;
      case SampleOptionTypes::EOpt1:
        return std::string("EOpt1") ;
      default:
        return std::string("Undefined") ;
      }
    }


} ;

}