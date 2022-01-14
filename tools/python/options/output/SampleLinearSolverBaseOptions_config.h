
/*
 * BaseOption.h Generated file
 */

#pragma once

#include <cxxopts.hpp>

namespace Alien {

class SampleLinearSolverBaseOptionsSetter ;

class SampleLinearSolverBaseOptionsEnvSetter ;

class SampleLinearSolverBaseOptionsXMLConfigSetter ;

class SampleLinearSolverBaseOptionsJSONConfigSetter ;

class SampleLinearSolverBaseOptions
{
public:
  class ComplexOptBaseOptions
  {
  public:
    ComplexOptBaseOptions(SampleLinearSolverBaseOptions const* parent)
    : m_parent(parent)
    {}

    ComplexOptBaseOptions const* operator ->() const {
        return this ;
    }

    static void init(cxxopts::Options& options)
    {
       options.add_options("Group options for complex SampleLinearSolver")
           // REGISTER SIMPLE OPTIONS
           ("complex-opt-sopt", " sopt integer option of complex complex-opt option ", cxxopts::value<Arccore::Integer>())
           // REGISTER ENUM OPTIONS
           ("complex-opt-eopt", "eopt enum option of complex complex-opt option", cxxopts::value<std::string>()->default_value("EOpt0"))
            ;
    }

    /*
     * ACCESSORS
     */
    // SIMPLE OPTIONS ACCESSORS
    //! sopt option accessor
    Arccore::Integer const& sopt() const {
         return m_parent->m_results["complex-opt-sopt"].as<Arccore::Integer>() ;
    }


    // ENUM OPTIONS ACCESSOR
    //! eopt option accessor
    SampleOptionTypes::eOpt eopt() const {
          return SamplelinearsolverBaseOptionsEnum::EoptStringToEnum(m_parent->m_results["complex-opt-eopt"].as<std::string>()) ;
    }


  private:
    SampleLinearSolverBaseOptions const* m_parent = nullptr ;
  } ;
  friend class ComplexOptBaseOptions ;

  // Constructor
  SampleLinearSolverBaseOptions()
  : m_options("SampleLinearSolverBaseOptions", "DataBase options for Service : SampleLinearSolver")
  {
    m_options.add_options()
      // REGISTER SIMPLE OPTIONS
      ("max-iteration-num", "max-iteration-num simple integer option", cxxopts::value<Arccore::Integer>()->default_value("1000"))
      ("stop-criteria-value", "stop-criteria-value simple real option", cxxopts::value<Arccore::Real>()->default_value("1e-6"))
      ("verbose", "verbose simple bool option", cxxopts::value<bool>()->default_value("false"))
      ("output-level", "output-level simple integer option", cxxopts::value<Arccore::Integer>()->default_value("0"))
      ("sopt", "sopt simple real option", cxxopts::value<std::vector<Arccore::Real>>())
      // REGISTER ENUM OPTIONS
      ("solver", "solver enum option", cxxopts::value<std::string>()->default_value("BiCGStab"))
      ("preconditioner", "preconditioner enum option", cxxopts::value<std::string>()->default_value("ILU0"))
      ;

    // REGISTER COMPLEX OPTIONS
    ComplexOptBaseOptions::init(m_options) ;

  }

  virtual ~SampleLinearSolverBaseOptions() = default ;

  void init(int argc, const char* const* argv)
  {
    m_results = m_options.parse(argc,argv) ;
  }

  template<typename OptionSetterT>
  void init(OptionSetterT const& setter)
  {
    init(setter.argc(),setter.argv()) ;
  }

  /*
   * ACCESSORS
   */
  // SIMPLE OPTIONS ACCESSORS
  //! max-iteration-num option accessor
  Arccore::Integer const& maxIterationNum() const {
      return m_results["max-iteration-num"].as<Arccore::Integer>() ;
  }

  //! stop-criteria-value option accessor
  Arccore::Real const& stopCriteriaValue() const {
      return m_results["stop-criteria-value"].as<Arccore::Real>() ;
  }

  //! verbose option accessor
  bool const& verbose() const {
      return m_results["verbose"].as<bool>() ;
  }

  //! output-level option accessor
  Arccore::Integer const& outputLevel() const {
      return m_results["output-level"].as<Arccore::Integer>() ;
  }

  //! sopt option accessor
  std::vector<Arccore::Real> const& sopt() const {
      return m_results["sopt"].as<std::vector<Arccore::Real>>() ;
  }


  // ENUM OPTIONS ACCESSOR
  //! solver option accessor
  SampleOptionTypes::eSolver solver() const {
      return SamplelinearsolverBaseOptionsEnum::SolverStringToEnum(m_results["solver"].as<std::string>()) ;
  }

  //! preconditioner option accessor
  SampleOptionTypes::ePreconditioner preconditioner() const {
      return SamplelinearsolverBaseOptionsEnum::PreconditionerStringToEnum(m_results["preconditioner"].as<std::string>()) ;
  }



  // COMPLEX OPTIONS ACCESSOR
  //! complex-opt option accessor
  ComplexOptBaseOptions complexOpt() const {
      return ComplexOptBaseOptions(this) ;
  }


private :
  cxxopts::Options m_options;

  cxxopts::ParseResult m_results;
} ;

}