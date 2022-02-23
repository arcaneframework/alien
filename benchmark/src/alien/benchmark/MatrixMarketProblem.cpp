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
// Created by chevalierc on 22/02/2022.
//

#include "MatrixMarketProblem.h"

#include <alien/distribution/MatrixDistribution.h>

namespace Alien::Benchmark
{

MatrixMarketProblem::MatrixMarketProblem(Arccore::MessagePassing::IMessagePassingMng* pm, const std::string& matrix_filename, const std::string& rhs_filename)
: m_matrix(Alien::Move::readFromMatrixMarket(pm, matrix_filename))
, m_rhs(Alien::Move::readFromMatrixMarket(m_matrix.distribution().rowDistribution(), rhs_filename))
{
}

Alien::Move::MatrixData MatrixMarketProblem::matrix() const
{
  return m_matrix.clone();
}

Alien::Move::VectorData MatrixMarketProblem::vector() const
{
  return m_rhs.clone();
}

} // namespace Alien::Benchmark