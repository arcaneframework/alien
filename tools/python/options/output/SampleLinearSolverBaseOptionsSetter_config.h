
/*
 * BaseOption.h Generated file
 */

#pragma once

#include <cxxopts.hpp>
#include <sstream>
#include <string>

#ifdef ALIEN_USE_LIBXML2
#include <alien/utils/XML2Tools.h>
#endif

#ifdef ALIEN_USE_BOOST_PROPERTY_TREE
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/optional/optional.hpp>
#endif

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

          std::stringstream value_key("") ;
          value_key<<value;
          m_parent->m_str_argv.push_back(value_key.str()) ;
    }


    // ENUM OPTIONS ACCESSOR
    //! eopt option setter
    void setEopt(SampleOptionTypes::eOpt value) const {
          m_parent->m_str_argv.push_back("--complex-opt-eopt") ;

          std::stringstream value_key("") ;
          value_key<<value;
          m_parent->m_str_argv.push_back(SamplelinearsolverBaseOptionsEnum::EoptEnumToString(value)) ;
    }


    void setEopt(std::string const& value) const {
          m_parent->m_str_argv.push_back("--complex-opt-eopt") ;

          m_parent->m_str_argv.push_back(value) ;
    }


      private:
      SampleLinearSolverBaseOptionsSetter* m_parent = nullptr ;
  } ;
  friend class ComplexOptBaseOptionSetter ;

  SampleLinearSolverBaseOptionsSetter(std::string const& name)
  : m_name(name)
  {
      m_str_argv.push_back(m_name) ;
  }

  ~SampleLinearSolverBaseOptionsSetter() = default ;

  // SETTER
  // SIMPLE OPTIONS SETTER
  //! max-iteration-num option setter
  void setMaxIterationNum(Arccore::Integer value) {
      m_str_argv.push_back("--max-iteration-num") ;

      std::stringstream value_key("") ;
      value_key<<value;
      m_str_argv.push_back(value_key.str()) ;
  }

  //! stop-criteria-value option setter
  void setStopCriteriaValue(Arccore::Real value) {
      m_str_argv.push_back("--stop-criteria-value") ;

      std::stringstream value_key("") ;
      value_key<<value;
      m_str_argv.push_back(value_key.str()) ;
  }

  //! verbose option setter
  void setVerbose(bool value) {
      m_str_argv.push_back("--verbose") ;

      std::stringstream value_key("") ;
      value_key<<value;
      m_str_argv.push_back(value_key.str()) ;
  }

  //! output-level option setter
  void setOutputLevel(Arccore::Integer value) {
      m_str_argv.push_back("--output-level") ;

      std::stringstream value_key("") ;
      value_key<<value;
      m_str_argv.push_back(value_key.str()) ;
  }

  //! sopt option setter
  void setSopt(Arccore::Real value) {
      m_str_argv.push_back("--sopt") ;

      std::stringstream value_key("") ;
      value_key<<value;
      m_str_argv.push_back(value_key.str()) ;
  }


  // ENUM OPTIONS SETTER
  //! solver option setter
  void setSolver(SampleOptionTypes::eSolver value) {
      m_str_argv.push_back("--solver") ;

      m_str_argv.push_back(SamplelinearsolverBaseOptionsEnum::SolverEnumToString(value)) ;
  }

  void setSolver(std::string const& value) {
      m_str_argv.push_back("--solver") ;

      m_str_argv.push_back(value) ;
  }

  //! preconditioner option setter
  void setPreconditioner(SampleOptionTypes::ePreconditioner value) {
      m_str_argv.push_back("--preconditioner") ;

      m_str_argv.push_back(SamplelinearsolverBaseOptionsEnum::PreconditionerEnumToString(value)) ;
  }

  void setPreconditioner(std::string const& value) {
      m_str_argv.push_back("--preconditioner") ;

      m_str_argv.push_back(value) ;
  }



  // COMPLEX OPTIONS ACCESSOR
  //! complex-opt option accessor
  ComplexOptBaseOptionSetter complexOptSetter() {
      return ComplexOptBaseOptionSetter(this) ;
  }


  std::vector<std::string> const& args() const {
    return m_str_argv ;
  }
protected :
  std::string              m_name ;
  std::vector<std::string> m_str_argv ;
} ;

class SampleLinearSolverBaseOptionsENVSetter
: public SampleLinearSolverBaseOptionsSetter
{
public:
  typedef SampleLinearSolverBaseOptionsSetter BaseType ;
  SampleLinearSolverBaseOptionsENVSetter(std::string const& name)
  : BaseType(name)
  {
    // SIMPLE OPTIONS SETTER
    //! max-iteration-num option setter
    {
      const char* value_key = std::getenv("ALIEN_SAMPLELINEARSOLVER.MAX_ITERATION_NUM") ;
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
      const char* value_key = std::getenv("ALIEN_SAMPLELINEARSOLVER.STOP_CRITERIA_VALUE") ;
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
      const char* value_key = std::getenv("ALIEN_SAMPLELINEARSOLVER.VERBOSE") ;
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
      const char* value_key = std::getenv("ALIEN_SAMPLELINEARSOLVER.OUTPUT_LEVEL") ;
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
      const char* value_key = std::getenv("ALIEN_SAMPLELINEARSOLVER.SOPT") ;
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
      const char* value_key = std::getenv("ALIEN_SAMPLELINEARSOLVER.SOLVER") ;
      if(value_key)
      {
         setSolver(std::string(value_key)) ;
      }
    }

    //! preconditioner option setter
    {
      const char* value_key = std::getenv("ALIEN_SAMPLELINEARSOLVER.PRECONDITIONER") ;
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
        const char* value_key = std::getenv("ALIEN_SAMPLELINEARSOLVER.COMPLEX_OPT.SOPT") ;
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
        const char* value_key = std::getenv("ALIEN_SAMPLELINEARSOLVER.COMPLEX_OPT.EOPT") ;
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

  SampleLinearSolverBaseOptionsXMLConfigSetter(std::string const& name, std::string const& config_file)
  : BaseType(name)
  , m_config_file(config_file)
  {
#ifdef ALIEN_USE_LIBXML2
    using namespace XML2Tools ;

    XMLDocument doc = openDocument(m_config_file) ;
    FileNode root_node = openFileNode(doc,name) ;

    // SIMPLE OPTIONS SETTER
    //! max-iteration-num option setter
    {
      Arccore::Integer value ;
      if(read(root_node,"max-iteration-num",value))
        setMaxIterationNum(value) ;
    }

    //! stop-criteria-value option setter
    {
      Arccore::Real value ;
      if(read(root_node,"stop-criteria-value",value))
        setStopCriteriaValue(value) ;
    }

    //! verbose option setter
    {
      bool value ;
      if(read(root_node,"verbose",value))
        setVerbose(value) ;
    }

    //! output-level option setter
    {
      Arccore::Integer value ;
      if(read(root_node,"output-level",value))
        setOutputLevel(value) ;
    }

    //! sopt option setter
    {
      Arccore::Real value ;
      if(read(root_node,"sopt",value))
        setSopt(value) ;
    }


    // ENUM OPTIONS ACCESSOR
    //! solver option setter
    {
      std::string value ;
      if(read(root_node,"solver",value))
        setSolver(value) ;
    }

    //! preconditioner option setter
    {
      std::string value ;
      if(read(root_node,"preconditioner",value))
        setPreconditioner(value) ;
    }

    // COMPLEX OPTIONS ACCESSOR
    //! complex-opt option setter
    {
      auto setter = complexOptSetter() ;

      FileNode opt_node = openFileNode(root_node,"complex-opt") ;
      //! sopt option setter
      {
         Arccore::Integer value ;
         if(read(opt_node,"sopt",value))
           setter.setSopt(value) ;
      }

      //! eopt option setter
      {
         std::string value ;
         if(read(opt_node,"eopt",value))
           setter.setEopt(value) ;
      }

    }

#endif
  }
private:

  std::string const& m_config_file ;
};

class SampleLinearSolverBaseOptionsJSONConfigSetter
: public SampleLinearSolverBaseOptionsSetter
{
public:
  typedef SampleLinearSolverBaseOptionsSetter BaseType ;
  SampleLinearSolverBaseOptionsJSONConfigSetter(std::string const& name, std::string const& config_file)
  : BaseType(name)
  , m_config_file(config_file)
  {
#ifdef ALIEN_USE_BOOST_PROPERTY_TREE
    namespace pt = boost::property_tree;
   // Create a root
    pt::ptree root;

    // Load the json file in this ptree
    pt::read_json(m_config_file, root);

    // SIMPLE OPTIONS SETTER
    //! max-iteration-num option setter
    {
      if( root.count("max-iteration-num") != 0 )
      {
         auto value = root.get<Arccore::Integer>("max-iteration-num") ;
         setMaxIterationNum(value) ;
      }
    }

    //! stop-criteria-value option setter
    {
      if( root.count("stop-criteria-value") != 0 )
      {
         auto value = root.get<Arccore::Real>("stop-criteria-value") ;
         setStopCriteriaValue(value) ;
      }
    }

    //! verbose option setter
    {
      if( root.count("verbose") != 0 )
      {
         auto value = root.get<bool>("verbose") ;
         setVerbose(value) ;
      }
    }

    //! output-level option setter
    {
      if( root.count("output-level") != 0 )
      {
         auto value = root.get<Arccore::Integer>("output-level") ;
         setOutputLevel(value) ;
      }
    }

    //! sopt option setter
    {
      if( root.count("sopt") != 0 )
      {
         auto value = root.get<Arccore::Real>("sopt") ;
         setSopt(value) ;
      }
    }


    // ENUM OPTIONS ACCESSOR
    //! solver option setter
    {
      if( root.count("solver") != 0 )
      {
         auto value = root.get<std::string>("solver") ;
         setSolver(value) ;
      }
    }

    //! preconditioner option setter
    {
      if( root.count("preconditioner") != 0 )
      {
         auto value = root.get<std::string>("preconditioner") ;
         setPreconditioner(value) ;
      }
    }



    // COMPLEX OPTIONS ACCESSOR
    //! complex-opt option setter
    {
      auto setter = complexOptSetter() ;

      if( root.count("complex-opt") != 0 )
      {
        //! sopt option setter
        {
            auto value = root.get<Arccore::Integer>("complex-opt.sopt") ;
            setter.setSopt(value) ;
        }

        //! eopt option setter
        {
            auto value = root.get<std::string>("complex-opt.eopt") ;
            setter.setEopt(value) ;
        }

      }

    }
#endif
  }

  ~SampleLinearSolverBaseOptionsJSONConfigSetter() = default ;

private:
  std::string const& m_config_file ;
};

} // end namespace Alien