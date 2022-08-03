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

#pragma once

#include <alien/utils/Precomp.h>
#include <arccore/base/String.h>
#include <arccore/base/ArccoreGlobal.h>

extern "C" int MPI_Init(int *argc, char ***argv);

#include <map>

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Alien
{

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef String BackEndId;

template <typename Tag>
struct AlgebraTraits;

template <typename Tag>
class LUSendRecvTraits;

class IInternalLinearSolver;

class ILinearSolver;
class ILinearAlgebra;

class IMatrixConverter;
class IVectorConverter;
class IMatrixImpl;
class IVectorImpl;

class MultiMatrixImpl;
class MultiVectorImpl;


namespace BackEnd
{
  struct OptionTypes
  {
    enum eSolver
    {
      AMG,
      CG,
      GMRES,
      BiCGStab,
      Hybrid
    };

    enum ePreconditioner
    {
      NoPC,
      DiagPC,
      AMGPC,
      ParaSailsPC,
      EuclidPC
    };
  };

  struct Options
  {
    Arccore::Integer numIterationsMax_ = 100;
    Arccore::Integer numIterationsMax() const { return numIterationsMax_; }
    Options& numIterationsMax(Arccore::Integer n)
    {
      numIterationsMax_ = n;
      return *this;
    }

    Arccore::Real stopCriteriaValue_ = 1.e-10;
    Arccore::Real stopCriteriaValue() const { return stopCriteriaValue_; }
    Options& stopCriteriaValue(Arccore::Real n)
    {
      stopCriteriaValue_ = n;
      return *this;
    }

    bool verbose_ = false;
    bool verbose() const { return verbose_; }
    Options& verbose(bool n)
    {
      verbose_ = n;
      return *this;
    }

    OptionTypes::eSolver solver_;
    OptionTypes::eSolver solver() const { return solver_; }
    Options& solver(OptionTypes::eSolver n)
    {
      solver_ = n;
      return *this;
    }

    OptionTypes::ePreconditioner preconditioner_;
    OptionTypes::ePreconditioner preconditioner() const { return preconditioner_; }
    Options& preconditioner(OptionTypes::ePreconditioner n)
    {
      preconditioner_ = n;
      return *this;
    }
  };

  typedef IMatrixImpl* (*MatrixFactory) (const MultiMatrixImpl*);
  typedef IVectorImpl* (*VectorFactory) (const MultiVectorImpl*);

  class IPlugin
  {
  public:
    virtual std::unique_ptr<IInternalLinearSolver> solver_factory() = 0;
    virtual std::unique_ptr<IInternalLinearSolver> solver_factory(const Alien::BackEnd::Options& options) = 0;

    virtual void init() = 0;

    virtual void registerMatrixConverters(std::map<std::pair<BackEndId, BackEndId>, IMatrixConverter*>& converters) = 0;
    virtual void registerVectorConverters(std::map<std::pair<BackEndId, BackEndId>, IVectorConverter*>& converters) = 0;
    virtual void registerMatrixFactory(std::map<BackEndId, MatrixFactory>& matrixFactories) = 0;
    virtual void registerVectorFactory(std::map<BackEndId, VectorFactory>& vectorFactories) = 0;

    virtual BackEndId name() = 0;
  };
} // namespace BackEnd

#define REGISTER_PLUGIN_MATRIX_CONVERTER(a, b, converter) \
  converters.insert(std::map<std::pair<BackEndId, BackEndId>, IMatrixConverter*>::value_type(std::pair<BackEndId, BackEndId>{a, b}, new converter()));

#define REGISTER_PLUGIN_VECTOR_CONVERTER(a, b, converter) \
  converters.insert(std::map<std::pair<BackEndId, BackEndId>, IVectorConverter*>::value_type(std::pair<BackEndId, BackEndId>{a, b}, new converter()));


#define REGISTER_PLUGIN_MATRIX_FACTORY(a, factories, factory) \
  factories.insert(std::map<BackEndId, Alien::BackEnd::MatrixFactory>::value_type(a, factory));

#define REGISTER_PLUGIN_VECTOR_FACTORY(a, factories, factory) \
  factories.insert(std::map<BackEndId, Alien::BackEnd::VectorFactory>::value_type(a, factory));


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // namespace Alien

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
