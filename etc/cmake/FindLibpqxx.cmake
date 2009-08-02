# Find the PostgreSQL installation or build tree.
# The following variables are set if LIBPQXX is found.  If LIBPQXX is not
# found, LIBPQXX_FOUND is set to false.
#  LIBPQXX_FOUND         - Set to true when LIBPQXX is found.
#  LIBPQXX_INCLUDE_DIRS  - Include directories for LIBPQXX
#  LIBPQXX_LIBRARY_DIRS  - Link directories for LIBPQXX libraries
# The following cache entries must be set by the user to locate LIBPQXX:
#  LIBPQXX_DIR  - The directory containing LIBPQXXConfig.cmake.
#             This is either the root of the build tree,
#             or the lib/LIBPQXX directory.  This is the
#             only cache entry.

#  LIBPQXX_FOUND        - True when the LIBPQXX include directory is found.
#  LIBPQXX_INCLUDE_DIRS - the path to where the LIBPQXX include files are.
#  LIBPQXX_LIBRARY_DIRS - The path to where the LIBPQXX library files are.

# ----------------------------------------------------------------------------
# If you have installed LIBPQXX in a non-standard location.
# Then you have three options. 
# In the following comments, it is assumed that <Your Path>
# points to the root directory of the include directory of postgresql. e.g
# If you have put postgresql in C:\development\postgresql then <Your Path> is
# "C:/development/postgresql" and in this directory there will be two
# directories called "include" and "lib".
# 1) After CMake runs, set LIBPQXX_INCLUDE_DIR to <Your Path>/LIBPQXX<-version>
# 2) Use CMAKE_INCLUDE_PATH to set a path to <Your Path>/LIBPQXX<-version>. This will allow FIND_PATH()
#    to locate LIBPQXX_INCLUDE_DIR by utilizing the PATH_SUFFIXES option. e.g.
#    SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "<Your Path>/include")
# 3) Set an environment variable called ${LIBPQXX_ROOT} that points to the root of where you have
#    installed LIBPQXX, e.g. <Your Path>. It is assumed that there is at least a subdirectory called
#    Foundation/include/LIBPQXX in this path.
#
# Usage:
# In your CMakeLists.txt file do something like this:
# ...
# # LIBPQXX
# FIND_PACKAGE(LIBPQXX)
# ...
# INCLUDE_DIRECTORIES(${LIBPQXX_INCLUDE_DIRS})
# LINK_DIRECTORIES(${LIBPQXX_LIBRARY_DIRS})
#
# In Windows, we make the assumption that, if the LIBPQXX files are installed, the default directory
# will be C:\postgresql or C:\Program Files\postgresql.

set(LIBPQXX_INCLUDE_PATH_DESCRIPTION "top-level directory containing the LIBPQXX include directories.")
set(LIBPQXX_INCLUDE_DIR_MESSAGE "Set the LIBPQXX_INCLUDE_DIR cmake cache entry to the ${LIBPQXX_INCLUDE_PATH_DESCRIPTION}")
set(LIBPQXX_LIBRARY_PATH_DESCRIPTION "top-level directory containing the LIBPQXX libraries.")
set(LIBPQXX_LIBRARY_DIR_MESSAGE "Set the LIBPQXX_LIBRARY_DIR cmake cache entry to the ${LIBPQXX_LIBRARY_PATH_DESCRIPTION}")

set(LIBPQXX_DIR_SEARCH $ENV{LIBPQXX_ROOT})
if(LIBPQXX_DIR_SEARCH)
  file(TO_CMAKE_PATH ${LIBPQXX_DIR_SEARCH} LIBPQXX_DIR_SEARCH)
endif(LIBPQXX_DIR_SEARCH)

# Add in some path suffixes. These will have to be updated whenever a new PostgreSQL version comes out.
set(SUFFIX_FOR_INCLUDE_PATH
  include
)

set(SUFFIX_FOR_LIBRARY_PATH
  lib
)

#
# Look for an installation.
#
find_path(LIBPQXX_INCLUDE_DIR NAMES pqxx/pqxx PATH_SUFFIXES ${SUFFIX_FOR_INCLUDE_PATH} PATHS

  # Look in other places.
  ${LIBPQXX_DIR_SEARCH}

  # Help the user find it if we cannot.
  DOC "The ${LIBPQXX_INCLUDE_DIR_MESSAGE}"
)

find_library(LIBPQXX_LIBRARIES
  NAMES
  pqxx
  libpqxx_static
  PATH_SUFFIXES
  ${SUFFIX_FOR_LIBRARY_PATH}
  PATHS
  ${LIBPQXX_DIR_SEARCH})

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LibPqxx DEFAULT_MSG LIBPQXX_INCLUDE_DIR LIBPQXX_LIBRARIES)

if(NOT LIBPQXX_FOUND)
  if(NOT LIBPQXX_FIND_QUIETLY)
    message(STATUS "LIBPQXX was not found. ${LIBPQXX_DIR_MESSAGE}")
  else(NOT LIBPQXX_FIND_QUIETLY)
    if(LIBPQXX_FIND_REQUIRED)
      message(FATAL_ERROR "LIBPQXX was not found. ${LIBPQXX_DIR_MESSAGE}")
    endif(LIBPQXX_FIND_REQUIRED)
  endif(NOT LIBPQXX_FIND_QUIETLY)
endif(NOT LIBPQXX_FOUND)
