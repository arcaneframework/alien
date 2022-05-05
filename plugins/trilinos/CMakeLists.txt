option(ALIEN_TRILINOS_UNIT_TESTS "Enable Trilinos wrapper tests" ${ALIEN_UNIT_TESTS})


find_package(MPI REQUIRED)


# ----------------------------------------------------------------------------
# Disable Kokkos warning about not supporting C++ extensions
message("---- Trilinos CMAKE starts here ! ----")
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Trilinos REQUIRED COMPONENTS Teuchos Kokkos KokkosKernels Belos Tpetra Ifpack2)

# Echo trilinos build info just for fun
MESSAGE("\nFound Trilinos!  Here are the details: ")
MESSAGE("   Trilinos_DIR = ${Trilinos_DIR}")
MESSAGE("   Trilinos_VERSION = ${Trilinos_VERSION}")
MESSAGE("   Trilinos_PACKAGE_LIST = ${Trilinos_PACKAGE_LIST}")
MESSAGE("   Trilinos_LIBRARIES = ${Trilinos_LIBRARIES}")
MESSAGE("   Trilinos_INCLUDE_DIRS = ${Trilinos_INCLUDE_DIRS}")
MESSAGE("   Trilinos_LIBRARY_DIRS = ${Trilinos_LIBRARY_DIRS}")
MESSAGE("   Trilinos_TPL_LIST = ${Trilinos_TPL_LIST}")
MESSAGE("   Trilinos_TPL_INCLUDE_DIRS = ${Trilinos_TPL_INCLUDE_DIRS}")
MESSAGE("   Trilinos_TPL_LIBRARIES = ${Trilinos_TPL_LIBRARIES}")
MESSAGE("   Trilinos_TPL_LIBRARY_DIRS = ${Trilinos_TPL_LIBRARY_DIRS}")
MESSAGE("   Trilinos_BUILD_SHARED_LIBS = ${Trilinos_BUILD_SHARED_LIBS}")
MESSAGE("End of Trilinos details\n")

# Make sure to use same compilers and flags as Trilinos
#MESSAGE("   CMAKE_CXX_COMPILER = ${CMAKE_CXX_COMPILER}")
#MESSAGE("   Trilinos_CXX_COMPILER = ${Trilinos_CXX_COMPILER}")
#MESSAGE("   CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
#MESSAGE("   Trilinos_CXX_COMPILER_FLAGS =  ${Trilinos_CXX_COMPILER_FLAGS} ")

#set(CMAKE_CXX_COMPILER ${Trilinos_CXX_COMPILER} )
set(CMAKE_CXX_FLAGS  "${Trilinos_CXX_COMPILER_FLAGS} ${CMAKE_CXX_FLAGS}")





# ----------------------------------------------------------------------------
# ----------------------------------------------------------------------------

add_library(trilinos_wrapper
        include/alien/trilinos/backend.h
        include/alien/trilinos/options.h
        src/trilinos_vector.cpp
        src/trilinos_vector.h
        src/trilinos_matrix.cpp
        src/trilinos_matrix.h
        src/trilinos_linear_algebra.cpp
        src/trilinos_linear_solver.cpp
        src/trilinos_linear_solver.h
        src/converters/trilinos_to_simplecsr_vector.cpp
        src/converters/simplecsr_to_trilinos_vector.cpp
        src/converters/simplecsr_to_trilinos_matrix.cpp

        )

target_link_libraries(trilinos_wrapper PUBLIC
        Alien::alien_core
        )

target_link_libraries(trilinos_wrapper PRIVATE
        MPI::MPI_CXX
        Arccore::arccore_trace
        Arccore::arccore_collections
        Arccore::arccore_base
        Arccore::arccore_message_passing_mpi
        ${Trilinos_LIBRARIES}
        ${Trilinos_TPL_LIBRARIES}
        )

target_include_directories(trilinos_wrapper PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        ${Trilinos_INCLUDE_DIRS}
        ${Trilinos_TPL_INCLUDE_DIRS}
        )

add_library(Alien::trilinos_wrapper ALIAS trilinos_wrapper)

include(GenerateExportHeader)

generate_export_header(trilinos_wrapper
        BASE_NAME ALIEN_TRILINOS
        EXPORT_FILE_NAME include/alien/trilinos/export.h
        )

# Examples
set(ALIEN_BENCHMARK ON)
add_subdirectory(examples)

# Tests
if (ALIEN_TRILINOS_UNIT_TESTS)
    #add_subdirectory(tests)
endif (ALIEN_TRILINOS_UNIT_TESTS)

# Install

include(ArcconSetInstallDirs)

set(ALIENTRILINOS_EXPORT_TARGET ${PROJECT_NAME}Targets)

install(DIRECTORY include/alien/trilinos
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/alien
        )

install(TARGETS trilinos_wrapper EXPORT ${ALIENTRILINOS_EXPORT_TARGET}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        )

install(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/include/alien/trilinos/export.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/alien/trilinos
)

install(EXPORT ${ALIENTRILINOS_EXPORT_TARGET}
        NAMESPACE ${namespace}
        DESTINATION ${config_install_dir})

install(DIRECTORY examples/ DESTINATION ${CMAKE_INSTALL_DOCDIR}/examples/trilinos)
# ----------------------------------------------------------------------------
# ----------------------------------------------------------------------------
