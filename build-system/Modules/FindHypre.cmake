include (${CMAKE_CURRENT_LIST_DIR}/../commands/commands.cmake)
arccon_return_if_package_found(Hypre)

# Les versions récentes de Hypre permettent d'utiliser un CMakeLists.txt
# Tente d'utiliser le module correspondant de CMake s'il existe.
# A noter que ce module s'appelle 'HYPRE' et pas 'Hypre'.

set(_SAVED_CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH})
unset(CMAKE_MODULE_PATH)
find_package(HYPRE CONFIG QUIET)
set(CMAKE_MODULE_PATH ${_SAVED_CMAKE_MODULE_PATH})
if (TARGET HYPRE::HYPRE)
  arccon_register_package_library(Hypre HYPRE)
  set(ARCCON_TARGET_Hypre HYPRE::HYPRE CACHE STRING "Target for package HYPRE" FORCE)
  return()
endif()

find_library(Hypre_LIBRARY
     NAMES HYPRE)

# On debian/ubuntu, headers can be found in a /usr/include/"pkg"
find_path(Hypre_INCLUDE_DIRS HYPRE.h
        PATH_SUFFIXES Hypre hypre)
mark_as_advanced(Hypre_INCLUDE_DIRS)

set(Hypre_INCLUDE_DIRS ${Hypre_INCLUDE_DIR})

# SD: others libraries should be found by find_library
# Look for other lib for Hypre
# It is the case on debian based platform
get_filename_component(Hypre_LIBRARY_DIR ${Hypre_LIBRARY} DIRECTORY)
# extra : .so .a .dylib, etc
file(GLOB Hypre_LIBRARIES ${Hypre_LIBRARY_DIR}/libHYPRE*.*)

message(STATUS "Hypre_INCLUDE_DIRS=${Hypre_INCLUDE_DIRS}")
message(STATUS "Hypre_LIBRARIES=${Hypre_LIBRARIES}")
message(STATUS "Hypre_LIBRARY=${Hypre_LIBRARY}")
message(STATUS "Hypre_LIBRARY_DIR=${Hypre_LIBRARY_DIR}")
mark_as_advanced(Hypre_LIBRARIES)

find_package_handle_standard_args(Hypre
        DEFAULT_MSG
        Hypre_INCLUDE_DIRS
        Hypre_LIBRARIES)

arccon_register_package_library(Hypre Hypre)

# Find the Hypre includes and library
#arccon_find_legacy_package(NAME Hypre LIBRARIES Hypre HEADERS Hypre.h)
