
# FindCrane - attempts to locate the Crane library.
#
# This module defines the following variables (on success):
#   CRANE_INCLUDE_DIRS - where to find Crane.hpp
#   CRANE_LIBRARY_DIRS - where to find Crane libraries
#   CRANE_FOUND        - if the library was successfully located
#
# It is trying a few standard installation locations, but can be customized
# with the following variables:
#   CRANE_ROOT_DIR     - root directory of a Crane installation
#                       Headers are expected to be found in:
#                       <CRANE_ROOT_DIR>/include
#                       Libraries are expected to be found in:
#                       <CRANE_ROOT_DIR>/libs
#                       This variable can either be a cmake or environment
#                       variable. Note however that changing the value
#                       of the environment varible will NOT result in
#                       re-running the header search and therefore NOT
#                       adjust the variables set by this module.

# default search dirs
SET(_crane_HEADER_SEARCH_DIRS
    "/usr/include"
    "/usr/local/include"
)

SET(_crane_LIBRARY_SEARCH_DIRS
    "/usr/lib"
    "/usr/lib64"
    "/usr/local/lib64"
)

# check environment variable
SET(_crane_ENV_ROOT_DIR "$ENV{CRANE_ROOT_DIR}")

IF(NOT CRANE_ROOT_DIR AND _crane_ENV_ROOT_DIR)
    SET(CRANE_ROOT_DIR "${_crane_ENV_ROOT_DIR}")
ENDIF(NOT CRANE_ROOT_DIR AND _crane_ENV_ROOT_DIR)

# put user specified location at beginning of search
IF(CRANE_ROOT_DIR)
    SET(_crane_HEADER_SEARCH_DIRS
        "${CRANE_ROOT_DIR}"
        "${CRANE_ROOT_DIR}/include"
        ${_crane_HEADER_SEARCH_DIRS}
    )

    SET(_crane_LIBRARY_SEARCH_DIRS
        "${CRANE_ROOT_DIR}"
        "${CRANE_ROOT_DIR}/lib"
        ${_crane_LIBRARY_SEARCH_DIRS}
    )
ENDIF(CRANE_ROOT_DIR)

# locate header
FIND_PATH(CRANE_INCLUDE_DIR 
    "Crane.hpp"
    PATHS ${_crane_HEADER_SEARCH_DIRS}
)

FIND_PATH(CRANE_LIBRARY_DIR 
    "libcrane.so"
    PATHS ${_crane_LIBRARY_SEARCH_DIRS}
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CRANE DEFAULT_MSG
    CRANE_INCLUDE_DIR CRANE_LIBRARY_DIR
)

if(CRANE_FOUND)
    SET(CRANE_INCLUDE_DIRS "${CRANE_INCLUDE_DIR}")
    SET(CRANE_LIBRARY_DIRS "${CRANE_LIBRARY_DIR}")
endif(CRANE_FOUND)

message(STATUS "CRANE_INCLUDE_DIRS = ${CRANE_INCLUDE_DIRS}")
message(STATUS "CRANE_LIBRARY_DIRS = ${CRANE_LIBRARY_DIRS}")


