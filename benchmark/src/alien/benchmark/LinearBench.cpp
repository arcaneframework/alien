/*
 * Copyright 2022 IFPEN-CEA
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
 *  SPDX-License-Identifier: Apache-2.0
 */

//
// Created by chevalierc on 23/02/22.
//

#include "LinearBench.h"

#include <alien/move/handlers/scalar/VectorWriter.h>

#include <alien/kernels/simple_csr/algebra/SimpleCSRLinearAlgebra.h>

namespace Alien::Benchmark
{
Alien::Move::VectorData LinearBench::solve(ILinearSolver* solver) const
{
  auto A = m_lp->matrix();
  auto b = m_lp->vector();
  /**
	 *  Préparation du solveur pour le calcul de x, tq : Ax = b
	 ********************************************/
  Alien::Move::VectorData x(A.distribution().colDistribution());

  // init vector x with zeros
  Alien::Move::LocalVectorWriter writer(std::move(x));
  for (int i = 0; i < writer.size(); i++) {
    writer[i] = 0;
  }
  x = writer.release();

  // solve
  solver->solve(A, b, x);

  return x;
}

LinearBenchResults::LinearBenchResults(const LinearBench& bench, Alien::Move::VectorData&& solution)
{
  SimpleCSRLinearAlgebra algebra;

  auto linop = bench.m_lp->matrix();

  Alien::Move::VectorData residual(linop.distribution().rowDistribution());

  auto rhs = bench.m_lp->vector();

  algebra.mult(linop, solution, residual);
  algebra.axpy(-1., rhs, residual);

  m_solution = computeAnalytics(solution);
  m_rhs = computeAnalytics(rhs);
}
} // namespace Alien::Benchmark
