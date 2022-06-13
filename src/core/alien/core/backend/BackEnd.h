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
} // namespace BackEnd





/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // namespace Alien

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
