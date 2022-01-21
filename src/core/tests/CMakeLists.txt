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

include(LoadAlienTest)
include(LoadAlienOptionsConfigGenerator)

find_package(MPI REQUIRED)
find_package(GTest REQUIRED)

configure_file(Sample.xml Sample.xml COPYONLY)
configure_file(Sample.json Sample.json COPYONLY)

# tests with GTest

add_executable(core.gtest.seq main.cpp
        TestSpace.cc TestAlienInclude.cc TestSpacePartition.cc)

add_executable(core.gtest.mpi main.cpp
        TestBackEnds.cc
        TestTimestamp.cc
        TestCompositeSpace.cc
        TestUniverse.cc
        TestDistribution.cc
        TestDoK.cc
        TestRedistributor.cc
        )

IF(ALIEN_GENERATE_OPTIONSCONFIG)
    target_sources(core.gtest.seq PRIVATE TestOptionsConfig.cc)

    generateOptionsConfig(core.gtest.seq
                          Sample.config
                          INSTALL_GENERATED_FILES
                         )
endif()

target_link_libraries(core.gtest.seq
        PUBLIC GTest::GTest
        PUBLIC Alien::alien_core
        alien_test_framework
        arcconpkg_MPI
        )

if (ALIEN_USE_LIBXML2)
    find_package(LibXml2 REQUIRED)

    target_link_libraries(core.gtest.seq PRIVATE arccon::LibXml2)
endif ()

if(ALIEN_USE_JSON)
    find_package(Boost COMPONENTS system)
    target_compile_definitions(core.gtest.seq PRIVATE ALIEN_USE_BOOST_PROPERTY_TREE)
    target_link_libraries(core.gtest.seq PRIVATE Boost::system)
endif()

target_link_libraries(core.gtest.mpi
    PUBLIC GTest::GTest
    PUBLIC Alien::alien_core
    alien_test_framework
    arcconpkg_MPI
    )

if (ALIEN_EXPAND_TESTS)
    gtest_discover_tests(core.gtest.seq TEST_PREFIX "core." TEST_SUFFIX ".seq")
    gtest_discover_tests(core.gtest.mpi TEST_PREFIX "core." TEST_SUFFIX ".seq")
else (ALIEN_EXPAND_TESTS)
    alien_test(
            BENCH core
            NAME "seq"
            COMMAND core.gtest.seq
    )
    alien_test(
            BENCH core
            NAME "mpi"
            COMMAND core.gtest.mpi
    )
endif (ALIEN_EXPAND_TESTS)

alien_test(
        BENCH core
        NAME "mpi"
        PROCS 2
        COMMAND core.gtest.mpi
        PARALLEL_ONLY
)
