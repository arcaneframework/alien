find_package(Ginkgo REQUIRED)

message("GINKGO----------------------------------------")
message("GINKGO :" ${GINKGO_INCLUDE_DIRS})

# ----------------------------------------------------------------------------

if (ALIEN_PLUGIN_GINKGO_OMP)
	message("------ COMPILING GINKGO *OPENMP* BACKEND  ------- ")
elseif (ALIEN_PLUGIN_GINKGO_CUDA)
	message("------ COMPILING GINKGO *CUDA* BACKEND  ------- ")
elseif (ALIEN_PLUGIN_GINKGO_HIP)
	message("------ COMPILING GINKGO *HIP* BACKEND  ------- ")
elseif (ALIEN_PLUGIN_GINKGO_DPCPP)
	message("------ COMPILING GINKGO *DPCPP* BACKEND  ------- ")
elseif (ALIEN_PLUGIN_GINKGO_REF)
	message("------ COMPILING GINKGO *REFERENCE* BACKEND  ------- ")
else()
	message("------ NO HARDWARE EXECUTOR SPECIFIED FOR GINKGO PLUGIN  ------- ")
	message("------ FALLBACK : COMPILING GINKGO *REF* BACKEND  ------- ")
endif()

configure_file(
		GinkgoConfig.h.in
		${CMAKE_CURRENT_BINARY_DIR}/include/alien/ginkgo/GinkgoConfig.h
)

# ----------------------------------------------------------------------------
add_library(alien_ginkgo
        src/matrix.h
        src/matrix.cpp
        src/vector.h
        src/vector.cpp
        src/ginkgo_linear_algebra.cpp
        src/ginkgo_linear_solver.cpp
        src/machine_backend.cpp
        src/converters/simplecsr_to_ginkgo_matrix.cpp
        src/converters/simplecsr_to_ginkgo_vector.cpp
        )

target_link_libraries(alien_ginkgo PUBLIC
	Alien::alien_core
	)

target_link_libraries(alien_ginkgo PRIVATE
        Arccore::arccore_trace
        Arccore::arccore_collections
        Arccore::arccore_base
        Arccore::arccore_message_passing_mpi
        arccon::Ginkgo
        )

target_include_directories(alien_ginkgo PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<BUILD_INTERFACE:${GINKGO_INCLUDE_DIRS}/..>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        )

add_library(Alien::alien_ginkgo ALIAS alien_ginkgo)

include(GenerateExportHeader)

generate_export_header(alien_ginkgo
        EXPORT_FILE_NAME include/alien/ginkgo/export.h
        )

# Examples
set (ALIEN_GINKGO_BENCHMARK ON)
add_subdirectory(bench)

# Tests
enable_testing()
add_subdirectory(test)

# Install

include(ArcconSetInstallDirs)

set(ALIENGINKGO_EXPORT_TARGET ${PROJECT_NAME}Targets)

install(DIRECTORY include/alien/ginkgo
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/alien
        )

install(TARGETS alien_ginkgo EXPORT ${ALIENGINKGO_EXPORT_TARGET}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        )

install(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/include/alien/ginkgo/export.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/alien/ginkgo
)

install(EXPORT ${ALIENGINKGO_EXPORT_TARGET}
        NAMESPACE ${namespace}
		DESTINATION ${config_install_dir})

# ----------------------------------------------------------------------------
# ----------------------------------------------------------------------------
