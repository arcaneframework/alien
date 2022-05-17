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

#include <alien/ginkgo/TrilinosConfig.h>
#include <Tpetra_Core.hpp>
#include <Tpetra_CrsMatrix.hpp>
#include <Ifpack2_Factory.hpp>



namespace Alien::Trilinos
{
#ifdef ALIEN_PLUGIN_TRILINOS_OMP
typedef Kokkos::Compat::KokkosOpenMPWrapperNode         Node;
#elif ALIEN_PLUGIN_TRILINOS_CUDA
typedef Kokkos::Compat::KokkosCudaWrapperNode           Node;
#elif ALIEN_PLUGIN_TRILINOS_HIP
typedef Kokkos::Compat::KokkosHIPWrapperNode            Node;
#elif ALIEN_PLUGIN_TRILINOS_SERIAL
typedef Kokkos::Compat::KokkosSerialWrapperNode         Node;
#elif ALIEN_PLUGIN_TRILINOS_THREADS
typedef Kokkos::Compat::KokkosThreadsWrapperNode        Node;
#elif ALIEN_PLUGIN_TRILINOS_SYCL
typedef Kokkos::Compat::KokkosSYCLWrapperNode           Node;
#else
typedef Kokkos::Compat::KokkosOpenMPWrapperNode         Node;
#endif

typedef double                                          SC;
typedef typename Tpetra::Map<>::local_ordinal_type      LO;
typedef typename Tpetra::Map<>::global_ordinal_type     GO;
typedef Tpetra::MultiVector<SC, LO, GO, Node>           MV;
typedef Tpetra::Operator<SC,LO,GO,Node>                 OP;
typedef Tpetra::Map<LO,GO,Node>                         map_type;
typedef Tpetra::CrsMatrix<SC,LO,GO,Node>                crs_matrix_type;
typedef Tpetra::RowMatrix<SC,LO,GO,Node>                row_matrix_type;
typedef Ifpack2::Preconditioner<SC,LO,GO,Node>          prec_type;

void kokkos_node_verbose();
}
