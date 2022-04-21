option(ALIEN_TRILINOS_UNIT_TESTS "Enable Trilinos wrapper tests" ${ALIEN_UNIT_TESTS})

find_package(Trilinos REQUIRED COMPONENTS Teuchos Kokkos KokkosKernels Belos Tpetra Ifpack2)

find_package(MPI REQUIRED)

# ----------------------------------------------------------------------------
# ----------------------------------------------------------------------------

add_library(trilinos_wrapper
        include/alien/trilinos/backend.h
        include/alien/trilinos/options.h
        src/trilinos_vector.cpp
        src/trilinos_vector.h
        src/trilinos_matrix.cpp
        src/trilinos_matrix.h
        #src/trilinos_linear_algebra.cpp
        #src/hypre_linear_solver.cpp
        #src/hypre_linear_solver.h
        #src/converters/hypre_to_simplecsr_vector.cpp
        #src/converters/simplecsr_to_hypre_vector.cpp
        #src/converters/simplecsr_to_hypre_matrix.cpp

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
        tpetra
        )


target_include_directories(trilinos_wrapper PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        )

add_library(Alien::trilinos_wrapper ALIAS trilinos_wrapper)

include(GenerateExportHeader)

generate_export_header(trilinos_wrapper
        BASE_NAME ALIEN_TRILINOS
        EXPORT_FILE_NAME include/alien/trilinos/export.h
        )

# Examples
set(ALIEN_BENCHMARK ON)
#add_subdirectory(examples)

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
