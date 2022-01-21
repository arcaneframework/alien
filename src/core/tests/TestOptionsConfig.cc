/*
 * Copyright 2020 IFPEN-CEA
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <gtest/gtest.h>

#include <Environment.h>
#include <iostream>
#include <utility>

#include <alien/utils/Precomp.h>
#include "SampleOptionTypes.h"
#include "SampleBaseOptionsEnum_config.h"
#include "SampleBaseOptionsSetter_config.h"
#include "SampleBaseOptions_config.h"

using namespace Alien;

TEST(TestOptionsConfig, InitCommandLine)
{
  int argc = 19 ;
  char const*argv[19] = { "SampleOption",
                          "--max-iter",          "500",
                          "--tol",               "1.e-6",
                          "--stop-criteria",     "RB",
                          "--solver",            "CG",
                          "--preconditioner",    "Chebyshev",
                          "--poly-order",        "3",
                          "--poly-factor",       "0.5",
                          "--poly-factor-iter",  "10",
                          "--complex-opt-sopt",  "100"} ;
  Alien::SampleBaseOptions sample_options{} ;
  sample_options.init(argc,argv) ;
}

TEST(TestOptionsConfig, SimpleOptions)
{
  int argc = 19 ;
  char const* argv[19] = { "SampleOption",
                           "--max-iter",          "500",
                           "--tol",               "1.e-6",
                           "--stop-criteria",     "RB",
                           "--solver",            "CG",
                           "--preconditioner",    "Chebyshev",
                           "--poly-order",        "3",
                           "--poly-factor",       "0.5",
                           "--poly-factor-iter",  "10",
                           "--complex-opt-sopt",  "100" } ;
  Alien::SampleBaseOptions sample_options{} ;
  sample_options.init(argc,argv) ;

  ASSERT_EQ(sample_options.maxIter(),500) ;
  ASSERT_EQ(sample_options.tol(),1.e-6) ;
  ASSERT_EQ(sample_options.polyOrder(),3) ;
  ASSERT_EQ(sample_options.polyFactor(),0.5) ;
  ASSERT_EQ(sample_options.polyFactorIter(),10) ;
}

TEST(TestOptionsConfig, EnumOptions)
{
  int argc = 19 ;
  char const*argv[19] = { "SampleOption",
                          "--max-iter",          "500",
                          "--tol",               "1.e-6",
                          "--stop-criteria",     "RB",
                          "--solver",            "CG",
                          "--preconditioner",    "Chebyshev",
                          "--poly-order",        "3",
                          "--poly-factor",       "0.5",
                          "--poly-factor-iter",  "10",
                          "--complex-opt-sopt",  "100" } ;
  Alien::SampleBaseOptions sample_options{} ;
  sample_options.init(argc,argv) ;

  ASSERT_EQ(sample_options.stopCriteria(),SampleOptionTypes::RB) ;
  ASSERT_EQ(sample_options.solver(),SampleOptionTypes::CG) ;
  ASSERT_EQ(sample_options.preconditioner(),SampleOptionTypes::ChebyshevPC) ;
}

TEST(TestOptionsConfig, ComplexOptions)
{
  int argc = 21 ;
  char const*argv[21] = { "SmapleSolver",
                          "--max-iter",          "500",
                          "--tol",               "1.e-6",
                          "--stop-criteria",     "RB",
                          "--solver",            "CG",
                          "--preconditioner",    "Chebyshev",
                          "--poly-order",        "3",
                          "--poly-factor",       "0.5",
                          "--poly-factor-iter",  "10",
                          "--complex-opt-sopt",  "100",
                          "--complex-opt-eopt",  "EOpt1"} ;
  Alien::SampleBaseOptions sample_options{} ;
  sample_options.init(argc,argv) ;

  ASSERT_EQ(sample_options.complexOpt().sopt(),100) ;
  ASSERT_EQ(sample_options.complexOpt().eopt(),SampleOptionTypes::EOpt1) ;
}

TEST(TestOptionsConfig, MissingOption)
{
  int argc = 17 ;
  char const*argv[17] = { "SampleOption",
                          "--max-iter",          "500",
                          "--tol",               "1.e-6",
                          "--stop-criteria",     "RB",
                          "--solver",            "CG",
                          "--preconditioner",    "Chebyshev",
                          "--poly-order",        "3",
                          "--poly-factor",       "0.5",
                          "--poly-factor-iter",  "10"} ;
  Alien::SampleBaseOptions sample_options{} ;
  try {
    sample_options.init(argc,argv) ;
    FAIL() << "Expected FatalErrorException" << std::endl ;
  }
  catch(Arccore::FatalErrorException const& exc)
  {
    EXPECT_EQ(exc.message(),Arccore::String("Error missing mandatory option : complex-opt-sopt "));
  }
  catch(...)
  {
    FAIL() << "Expected FatalErrorException" << std::endl ;
  }
}

TEST(TestOptionsConfig, InitFromSetter)
{

  Alien::SampleBaseOptions sample_options{} ;
  Alien::SampleBaseOptionsSetter setter("SampleSolver") ;

  setter.setMaxIter(500) ;
  setter.setTol(1.e-6) ;

  setter.setStopCriteria(SampleOptionTypes::RB) ;
  setter.setSolver(SampleOptionTypes::CG) ;
  setter.setPreconditioner(SampleOptionTypes::ChebyshevPC) ;

  setter.setPolyOrder(3) ;
  setter.setPolyFactor(0.5) ;
  setter.setPolyFactorIter(10) ;

  setter.complexOptSetter().setSopt(100) ;
  setter.complexOptSetter().setEopt(SampleOptionTypes::EOpt1) ;

  sample_options.init(setter) ;

  ASSERT_EQ(sample_options.maxIter(),500) ;
  ASSERT_EQ(sample_options.tol(),1.e-6) ;

  ASSERT_EQ(sample_options.stopCriteria(),SampleOptionTypes::RB) ;
  ASSERT_EQ(sample_options.solver(),SampleOptionTypes::CG) ;
  ASSERT_EQ(sample_options.preconditioner(),SampleOptionTypes::ChebyshevPC) ;

  ASSERT_EQ(sample_options.polyOrder(),3) ;
  ASSERT_EQ(sample_options.polyFactor(),0.5) ;
  ASSERT_EQ(sample_options.polyFactorIter(),10) ;

  ASSERT_EQ(sample_options.complexOpt().sopt(),100) ;
  ASSERT_EQ(sample_options.complexOpt().eopt(),SampleOptionTypes::EOpt1) ;
}


TEST(TestOptionsConfig, InitFromEnv)
{
  setenv("ALIEN_SAMPLE.MAX_ITER","500",1) ;
  setenv("ALIEN_SAMPLE.TOL","1.e-6",1) ;
  setenv("ALIEN_SAMPLE.STOP_CRITERIA","RB",1) ;
  setenv("ALIEN_SAMPLE.SOLVER","CG",1) ;
  setenv("ALIEN_SAMPLE.PRECONDITIONER","Chebyshev",1) ;
  setenv("ALIEN_SAMPLE.POLY_ORDER","3",1) ;
  setenv("ALIEN_SAMPLE.POLY_FACTOR","0.5",1) ;
  setenv("ALIEN_SAMPLE.POLY_FACTOR_ITER","10",1) ;
  setenv("ALIEN_SAMPLE.COMPLEX_OPT.SOPT","100",1) ;
  setenv("ALIEN_SAMPLE.COMPLEX_OPT.EOPT","EOpt1",1) ;

  Alien::SampleBaseOptions sample_options{} ;
  Alien::SampleBaseOptionsENVSetter setter("SampleSolver") ;
  sample_options.init(setter) ;

  ASSERT_EQ(sample_options.maxIter(),500) ;
  ASSERT_EQ(sample_options.tol(),1.e-6) ;

  ASSERT_EQ(sample_options.stopCriteria(),SampleOptionTypes::RB) ;
  ASSERT_EQ(sample_options.solver(),SampleOptionTypes::CG) ;
  ASSERT_EQ(sample_options.preconditioner(),SampleOptionTypes::ChebyshevPC) ;

  ASSERT_EQ(sample_options.polyOrder(),3) ;
  ASSERT_EQ(sample_options.polyFactor(),0.5) ;
  ASSERT_EQ(sample_options.polyFactorIter(),10) ;

  ASSERT_EQ(sample_options.complexOpt().sopt(),100) ;
  ASSERT_EQ(sample_options.complexOpt().eopt(),SampleOptionTypes::EOpt1) ;
}

#ifdef ALIEN_USE_LIBXML2
TEST(TestOptionsConfig, InitFromXmlConfig)
{
  Alien::SampleBaseOptions sample_options{} ;
  Alien::SampleBaseOptionsXMLConfigSetter setter("SampleSolver","Sample.xml") ;
  sample_options.init(setter) ;

  ASSERT_EQ(sample_options.maxIter(),500) ;
  ASSERT_EQ(sample_options.tol(),1.e-6) ;

  ASSERT_EQ(sample_options.stopCriteria(),SampleOptionTypes::RB) ;
  ASSERT_EQ(sample_options.solver(),SampleOptionTypes::CG) ;
  ASSERT_EQ(sample_options.preconditioner(),SampleOptionTypes::ChebyshevPC) ;

  ASSERT_EQ(sample_options.polyOrder(),3) ;
  ASSERT_EQ(sample_options.polyFactor(),0.5) ;
  ASSERT_EQ(sample_options.polyFactorIter(),10) ;

  ASSERT_EQ(sample_options.complexOpt().sopt(),100) ;
  ASSERT_EQ(sample_options.complexOpt().eopt(),SampleOptionTypes::EOpt1) ;
}
#endif


#ifdef ALIEN_USE_BOOST_PROPERTY_TREE
TEST(TestOptionsConfig, InitFromJsonConfig)
{
  Alien::SampleBaseOptions sample_options{} ;
  Alien::SampleBaseOptionsJSONConfigSetter setter("SampleSolver","Sample.json") ;
  sample_options.init(setter) ;

  ASSERT_EQ(sample_options.maxIter(),500) ;
  ASSERT_EQ(sample_options.tol(),1.e-6) ;

  ASSERT_EQ(sample_options.stopCriteria(),SampleOptionTypes::RB) ;
  ASSERT_EQ(sample_options.solver(),SampleOptionTypes::CG) ;
  ASSERT_EQ(sample_options.preconditioner(),SampleOptionTypes::ChebyshevPC) ;

  ASSERT_EQ(sample_options.polyOrder(),3) ;
  ASSERT_EQ(sample_options.polyFactor(),0.5) ;
  ASSERT_EQ(sample_options.polyFactorIter(),10) ;

  ASSERT_EQ(sample_options.complexOpt().sopt(),100) ;
  ASSERT_EQ(sample_options.complexOpt().eopt(),SampleOptionTypes::EOpt1) ;
}
#endif
