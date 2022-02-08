arccon_return_if_package_found(Ginkgo)
find_path(GINKGO_INCLUDE_DIRS ginkgo.hpp)

set (GINKGO_LIB_PATH  "/path/to/ginkgo/lib")
set (GINKGO_INC_PATH  "/path/to/ginkgo/include/ginkgo")

# On debian/ubuntu, headers can be found in a /usr/include/"pkg"
find_path(GINKGO_INCLUDE_DIRS ginkgo.hpp
	PATHS ${GINKGO_INC_PATH})

find_library(GINKGO_LIB ginkgo ${GINKGO_LIB_PATH})
find_library(GINKGO_HIP ginkgo_hip ${GINKGO_LIB_PATH})
find_library(GINKGO_DPCPP ginkgo_dpcpp ${GINKGO_LIB_PATH})
find_library(GINKGO_CUDA ginkgo_cuda ${GINKGO_LIB_PATH})
find_library(GINKGO_REF ginkgo_reference ${GINKGO_LIB_PATH})
find_library(GINKGO_DEVICE ginkgo_device ${GINKGO_LIB_PATH})
find_library(GINKGO_OMP ginkgo_omp PATHS ${GINKGO_LIB_PATH})

if (GINKGO_LIB)
  set(GINKGO_LIBRARIES ${GINKGO_LIB})
endif()

if (GINKGO_HIP)
  list(APPEND GINKGO_LIBRARIES ${GINKGO_HIP})
endif()

if (GINKGO_DPCPP)
  list(APPEND GINKGO_LIBRARIES ${GINKGO_DPCPP})
endif()

if (GINKGO_CUDA)
  list(APPEND GINKGO_LIBRARIES ${GINKGO_CUDA})
endif()

if (GINKGO_REF)
  list(APPEND GINKGO_LIBRARIES ${GINKGO_REF})
endif()

if (GINKGO_DEVICE)
  list(APPEND GINKGO_LIBRARIES ${GINKGO_DEVICE})
endif()

if (GINKGO_OMP)
  list(APPEND GINKGO_LIBRARIES ${GINKGO_OMP})
endif()


message(STATUS "GINKGO_INCLUDE_DIRS=" ${GINKGO_INCLUDE_DIRS})
message(STATUS "GINKGO_LIBRARIES=" ${GINKGO_LIBRARIES} )

unset(GINKGO_FOUND)
if (GINKGO_INCLUDE_DIRS AND GINKGO_LIBRARIES)
  set(GINKGO_FOUND TRUE)
  arccon_register_package_library(Ginkgo GINKGO)
endif()
