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

#include "../hypre_matrix.h"

#include <arccore/collections/Array2.h>

#include <alien/core/backend/IMatrixConverter.h>
#include <alien/core/backend/MatrixConverterRegisterer.h>
#include <alien/kernels/simple_csr/SimpleCSRMatrix.h>
#include <alien/kernels/simple_csr/SimpleCSRBackEnd.h>

#include <alien/hypre/backend.h>

class SimpleCSR_to_Hypre_MatrixConverter : public Alien::IMatrixConverter
{
 public:
  SimpleCSR_to_Hypre_MatrixConverter() {}

  virtual ~SimpleCSR_to_Hypre_MatrixConverter() {}

 public:
  BackEndId sourceBackend() const
  {
    return Alien::AlgebraTraits<Alien::BackEnd::tag::simplecsr>::name();
  }

  BackEndId targetBackend() const { return "hypre"; }

  void convert(const Alien::IMatrixImpl* sourceImpl, Alien::IMatrixImpl* targetImpl) const;

  void _build(const Alien::SimpleCSRMatrix<Arccore::Real>& sourceImpl, Alien::Matrix& targetImpl) const;

  void _buildBlock(const Alien::SimpleCSRMatrix<Arccore::Real>& sourceImpl, Alien::Matrix& targetImpl) const;
};
