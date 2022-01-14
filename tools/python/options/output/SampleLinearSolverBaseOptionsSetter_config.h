
/*
 * BaseOption.h Generated file
 */

#pragma once

#include <cxxopts.hpp>
#include <sstream>
#include <string>

namespace Alien {

class SampleLinearSolverBaseOptionsSetter
{
public :
  class ComplexOptBaseOptionSetter
  {
  public:
    ComplexOptBaseOptionSetter(SampleLinearSolverBaseOptionsSetter* parent)
    : m_parent(parent)
    {}

    ComplexOptBaseOptionSetter* operator ->() {
      return this ;
    }

    // ACCESSORS
    // SIMPLE OPTIONS SETTER
    //! sopt option setter
    void setSopt(Arccore::Integer value) {
          m_parent->m_str_argv.push_back("--complex-opt-sopt") ;
          m_parent->m_argv.push_back(m_parent->m_str_argv[m_parent->m_argc++].c_str()) ;

          std::stringstream value_key("") ;
          value_key<<value;
          m_parent->m_str_argv.push_back(value_key.str()) ;
          m_parent->m_argv.push_back(m_parent->m_str_argv[m_parent->m_argc++].c_str()) ;
    }


    // ENUM OPTIONS ACCESSOR
    //! eopt option setter
    void setEopt(SampleOptionTypes::eOpt value) const {
          m_parent->m_str_argv.push_back("--complex-opt-eopt") ;
          m_parent->m_argv.push_back(m_parent->m_str_argv[m_parent->m_argc++].c_str()) ;

          std::stringstream value_key("") ;
          value_key<<value;
          m_parent->m_str_argv.push_back(SamplelinearsolverBaseOptionsEnum::EoptEnumToString(value)) ;
          m_parent->m_argv.push_back(m_parent->m_str_argv[m_parent->m_argc++].c_str()) ;
    }


    void setEopt(std::string const& value) const {
          m_parent->m_str_argv.push_back("--complex-opt-eopt") ;
          m_parent->m_argv.push_back(m_parent->m_str_argv[m_parent->m_argc++].c_str()) ;

          m_parent->m_str_argv.push_back(value) ;
          m_parent->m_argv.push_back(m_parent->m_str_argv[m_parent->m_argc++].c_str()) ;
    }


      private:
      SampleLinearSolverBaseOptionsSetter* m_parent = nullptr ;
  } ;
  friend class ComplexOptBaseOptionSetter ;

  SampleLinearSolverBaseOptionsSetter() = default ;
  ~SampleLinearSolverBaseOptionsSetter() = default ;

  // SETTER
  // SIMPLE OPTIONS SETTER
  //! max-iteration-num option setter
  void setMaxIterationNum(Arccore::Integer value) {
      m_str_argv.push_back("--max-iteration-num") ;
      m_argv.push_back(m_str_argv[m_argc++].c_str()) ;

      std::stringstream value_key("") ;
      value_key<<value;
      m_str_argv.push_back(value_key.str()) ;
      m_argv.push_back(m_str_argv[m_argc++].c_str()) ;
  }

  //! stop-criteria-value option setter
  void setStopCriteriaValue(Arccore::Real value) {
      m_str_argv.push_back("--stop-criteria-value") ;
      m_argv.push_back(m_str_argv[m_argc++].c_str()) ;

      std::stringstream value_key("") ;
      value_key<<value;
      m_str_argv.push_back(value_key.str()) ;
      m_argv.push_back(m_str_argv[m_argc++].c_str()) ;
  }

  //! verbose option setter
  void setVerbose(bool value) {
      m_str_argv.push_back("--verbose") ;
      m_argv.push_back(m_str_argv[m_argc++].c_str()) ;

      std::stringstream value_key("") ;
      value_key<<value;
      m_str_argv.push_back(value_key.str()) ;
      m_argv.push_back(m_str_argv[m_argc++].c_str()) ;
  }

  //! output-level option setter
  void setOutputLevel(Arccore::Integer value) {
      m_str_argv.push_back("--output-level") ;
      m_argv.push_back(m_str_argv[m_argc++].c_str()) ;

      std::stringstream value_key("") ;
      value_key<<value;
      m_str_argv.push_back(value_key.str()) ;
      m_argv.push_back(m_str_argv[m_argc++].c_str()) ;
  }

  //! sopt option setter
  void setSopt(Arccore::Real value) {
      m_str_argv.push_back("--sopt") ;
      m_argv.push_back(m_str_argv[m_argc++].c_str()) ;

      std::stringstream value_key("") ;
      value_key<<value;
      m_str_argv.push_back(value_key.str()) ;
      m_argv.push_back(m_str_argv[m_argc++].c_str()) ;
  }


  // ENUM OPTIONS SETTER
  //! solver option setter
  void setSolver(SampleOptionTypes::eSolver value) {
      m_str_argv.push_back("--solver") ;
      m_argv.push_back(m_str_argv[m_argc++].c_str()) ;

      m_str_argv.push_back(SamplelinearsolverBaseOptionsEnum::SolverEnumToString(value)) ;
      m_argv.push_back(m_str_argv[m_argc++].c_str()) ;
  }

  void setSolver(std::string const& value) {
      m_str_argv.push_back("--solver") ;
      m_argv.push_back(m_str_argv[m_argc++].c_str()) ;

      m_str_argv.push_back(value) ;
      m_argv.push_back(m_str_argv[m_argc++].c_str()) ;
  }

  //! preconditioner option setter
  void setPreconditioner(SampleOptionTypes::ePreconditioner value) {
      m_str_argv.push_back("--preconditioner") ;
      m_argv.push_back(m_str_argv[m_argc++].c_str()) ;

      m_str_argv.push_back(SamplelinearsolverBaseOptionsEnum::PreconditionerEnumToString(value)) ;
      m_argv.push_back(m_str_argv[m_argc++].c_str()) ;
  }

  void setPreconditioner(std::string const& value) {
      m_str_argv.push_back("--preconditioner") ;
      m_argv.push_back(m_str_argv[m_argc++].c_str()) ;

      m_str_argv.push_back(value) ;
      m_argv.push_back(m_str_argv[m_argc++].c_str()) ;
  }



  // COMPLEX OPTIONS ACCESSOR
  //! complex-opt option accessor
  ComplexOptBaseOptionSetter complexOptSetter() {
      return ComplexOptBaseOptionSetter(this) ;
  }


  int argc() const {
    return m_argc ;
  }

  char const* const* argv() const {
    return m_argv.data() ;
  }
protected :
  int m_argc = 0 ;

  std::vector<char const*> m_argv ;

  std::vector<std::string> m_str_argv ;
} ;

class SampleLinearSolverBaseOptionsENVSetter
: public SampleLinearSolverBaseOptionsSetter
{
public:
  typedef SampleLinearSolverBaseOptionsSetter BaseType ;
  SampleLinearSolverBaseOptionsENVSetter()
  : BaseType()
  {
    // SIMPLE OPTIONS SETTER
    //! max-iteration-num option setter
    {
      const char* value_key = std::getenv("ALIEN_SAMPLELINEARSOLVER_MAX_ITERATION_NUM") ;
      if(value_key)
      {
         Arccore::Integer value ;
          std::istringstream iss(value_key) ;
          iss >> value ;
          setMaxIterationNum(value) ;
      }
    }

    //! stop-criteria-value option setter
    {
      const char* value_key = std::getenv("ALIEN_SAMPLELINEARSOLVER_STOP_CRITERIA_VALUE") ;
      if(value_key)
      {
         Arccore::Real value ;
          std::istringstream iss(value_key) ;
          iss >> value ;
          setStopCriteriaValue(value) ;
      }
    }

    //! verbose option setter
    {
      const char* value_key = std::getenv("ALIEN_SAMPLELINEARSOLVER_VERBOSE") ;
      if(value_key)
      {
         bool value ;
          std::istringstream iss(value_key) ;
          iss >> value ;
          setVerbose(value) ;
      }
    }

    //! output-level option setter
    {
      const char* value_key = std::getenv("ALIEN_SAMPLELINEARSOLVER_OUTPUT_LEVEL") ;
      if(value_key)
      {
         Arccore::Integer value ;
          std::istringstream iss(value_key) ;
          iss >> value ;
          setOutputLevel(value) ;
      }
    }

    //! sopt option setter
    {
      const char* value_key = std::getenv("ALIEN_SAMPLELINEARSOLVER_SOPT") ;
      if(value_key)
      {
         Arccore::Real value ;
          std::istringstream iss(value_key) ;
          iss >> value ;
          setSopt(value) ;
      }
    }


    // ENUM OPTIONS ACCESSOR
    //! solver option setter
    {
      const char* value_key = std::getenv("ALIEN_SAMPLELINEARSOLVER_SOLVER") ;
      if(value_key)
      {
         setSolver(std::string(value_key)) ;
      }
    }

    //! preconditioner option setter
    {
      const char* value_key = std::getenv("ALIEN_SAMPLELINEARSOLVER_PRECONDITIONER") ;
      if(value_key)
      {
         setPreconditioner(std::string(value_key)) ;
      }
    }



    // COMPLEX OPTIONS ACCESSOR
    //! complex-opt option setter
    {
      auto setter = complexOptSetter() ;

      //! sopt option setter
      {
        const char* value_key = std::getenv("ALIEN_SAMPLELINEARSOLVER_COMPLEX_OPT.SOPT") ;
        if(value_key)
        {
           Arccore::Integer value ;
           std::istringstream iss(value_key) ;
           iss >> value ;
           setter.setSopt(value) ;
        }
      }

      //! eopt option setter
      {
        const char* value_key = std::getenv("ALIEN_SAMPLELINEARSOLVER_COMPLEX_OPT.EOPT") ;
        if(value_key)
        {
           setter.setEopt(std::string(value_key)) ;
        }
      }

    }
  }
};

class SampleLinearSolverBaseOptionsXMLConfigSetter
: public SampleLinearSolverBaseOptionsSetter
{
public:
  typedef SampleLinearSolverBaseOptionsSetter BaseType ;
  SampleLinearSolverBaseOptionsXMLConfigSetter(std::string const& config_file)
  : BaseType()
  , m_config_file(config_file)
  {
  }
private:
  std::string const& m_config_file ;
};

class SampleLinearSolverBaseOptionsJSONConfigSetter
: public SampleLinearSolverBaseOptionsSetter
{
public:
  typedef SampleLinearSolverBaseOptionsSetter BaseType ;
  SampleLinearSolverBaseOptionsJSONConfigSetter(std::string const& config_file)
  : BaseType()
  , m_config_file(config_file)
  {
  }

  ~SampleLinearSolverBaseOptionsJSONConfigSetter() = default ;

private:
  std::string const& m_config_file ;
};

} // end namespace Alien