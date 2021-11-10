# Copyright 2020 IFPEN-CEA
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
# http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# 
# SPDX-License-Identifier: Apache-2.0

find_package(MPI REQUIRED)
find_package(GTest REQUIRED)

add_executable(move.gtest.mpi
        main.cpp
        TestIndexManager.cc
        TestDoKMatrix.cc
        )

add_executable(move.gtest.seq main.cpp
        TestVector.cc
        TestVectorBuilder.cc
        TestMatrixDirectBuilder.cc
)

target_link_libraries(move.gtest.mpi PRIVATE
        GTest::GTest
        Alien::alien_core
        Alien::alien_semantic_move
        alien_test_framework
        arcconpkg_MPI
        )

target_link_libraries(move.gtest.seq PRIVATE
        GTest::GTest
        Alien::alien_core
        Alien::alien_semantic_move
        alien_test_framework
        )

alien_test(
        BENCH move
        NAME "seq"
        COMMAND move.gtest.seq
)

alien_test(
        BENCH move
        NAME "mpi"
        PROCS 4
        COMMAND move.gtest.mpi
)
