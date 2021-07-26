if (Arccon_FOUND)
  return()
endif()

if (NOT ARCCON_EXPORT_TARGET)
  set(ARCCON_EXPORT_TARGET ${ARCCORE_EXPORT_TARGET})
endif()
SET(ARCCON_CMAKE_COMMANDS ${ARCCON_SRC_DIR}/Arccon.cmake)
set(ARCCON_MODULE_PATH ${ARCCON_SRC_DIR}/Modules)

list(APPEND CMAKE_MODULE_PATH ${ARCCON_MODULE_PATH})
include(${ARCCON_CMAKE_COMMANDS})
