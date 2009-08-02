# Find the PostgreSQL installation or build tree.
# The following variables are set if POSTGRESQL is found.  If POSTGRESQL is not
# found, POSTGRESQL_FOUND is set to false.
#  POSTGRESQL_FOUND         - Set to true when POSTGRESQL is found.
#  POSTGRESQL_INCLUDE_DIRS  - Include directories for POSTGRESQL
#  POSTGRESQL_LIBRARY_DIRS  - Link directories for POSTGRESQL libraries
# The following cache entries must be set by the user to locate POSTGRESQL:
#  POSTGRESQL_DIR  - The directory containing POSTGRESQLConfig.cmake.
#             This is either the root of the build tree,
#             or the lib/POSTGRESQL directory.  This is the
#             only cache entry.

#  POSTGRESQL_FOUND        - True when the POSTGRESQL include directory is found.
#  POSTGRESQL_INCLUDE_DIRS - the path to where the POSTGRESQL include files are.
#  POSTGRESQL_LIBRARY_DIRS - The path to where the POSTGRESQL library files are.

# ----------------------------------------------------------------------------
# If you have installed POSTGRESQL in a non-standard location.
# Then you have three options. 
# In the following comments, it is assumed that <Your Path>
# points to the root directory of the include directory of postgresql. e.g
# If you have put postgresql in C:\development\postgresql then <Your Path> is
# "C:/development/postgresql" and in this directory there will be two
# directories called "include" and "lib".
# 1) After CMake runs, set POSTGRESQL_INCLUDE_DIR to <Your Path>/POSTGRESQL<-version>
# 2) Use CMAKE_INCLUDE_PATH to set a path to <Your Path>/POSTGRESQL<-version>. This will allow FIND_PATH()
#    to locate POSTGRESQL_INCLUDE_DIR by utilizing the PATH_SUFFIXES option. e.g.
#    SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "<Your Path>/include")
# 3) Set an environment variable called ${POSTGRESQL_ROOT} that points to the root of where you have
#    installed POSTGRESQL, e.g. <Your Path>. It is assumed that there is at least a subdirectory called
#    Foundation/include/POSTGRESQL in this path.
#
# Usage:
# In your CMakeLists.txt file do something like this:
# ...
# # POSTGRESQL
# FIND_PACKAGE(POSTGRESQL)
# ...
# INCLUDE_DIRECTORIES(${POSTGRESQL_INCLUDE_DIRS})
# LINK_DIRECTORIES(${POSTGRESQL_LIBRARY_DIRS})
#
# In Windows, we make the assumption that, if the POSTGRESQL files are installed, the default directory
# will be C:\postgresql or C:\Program Files\postgresql.

set(POSTGRESQL_INCLUDE_PATH_DESCRIPTION "top-level directory containing the POSTGRESQL include directories. E.g /usr/local/include/postgresql/8.1 or C:/postgresql/postgresql-snapshot/src/include")
set(POSTGRESQL_INCLUDE_DIR_MESSAGE "Set the POSTGRESQL_INCLUDE_DIR cmake cache entry to the ${POSTGRESQL_INCLUDE_PATH_DESCRIPTION}")
set(POSTGRESQL_LIBRARY_PATH_DESCRIPTION "top-level directory containing the POSTGRESQL libraries.")
set(POSTGRESQL_LIBRARY_DIR_MESSAGE "Set the POSTGRESQL_LIBRARY_DIR cmake cache entry to the ${POSTGRESQL_LIBRARY_PATH_DESCRIPTION}")


set(POSTGRESQL_DIR_SEARCH $ENV{POSTGRESQL_ROOT})
if(POSTGRESQL_DIR_SEARCH)
  file(TO_CMAKE_PATH ${POSTGRESQL_DIR_SEARCH} POSTGRESQL_DIR_SEARCH)
endif(POSTGRESQL_DIR_SEARCH)

if(WIN32)
  set(POSTGRESQL_DIR_SEARCH
    ${POSTGRESQL_DIR_SEARCH}
    C:/postgresql
    D:/postgresql
    C:/postgresql/postgresql-8.3.5
    C:/postgresql/postgresql-8.3.1
    D:/postgresql/postgresql-8.3.1
    C:/postgresql/postgresql-8.2.4
    D:/postgresql/postgresql-8.2.4
    C:/postgresql/postgresql-snapshot
    D:/postgresql/postgresql-snapshot
    "C:/Program Files/postgresql"
    "C:/Program Files/postgresql/*"
    "D:/Program Files/postgresql"
    "D:/Program Files/postgresql/*"
  )
endif(WIN32)
  
if(UNIX)
  set(POSTGRESQL_DIR_SEARCH
    "/usr/include"
    "/usr/local/include")
endif(UNIX)

# Add in some path suffixes. These will have to be updated whenever a new PostgreSQL version comes out.
set(SUFFIX_FOR_INCLUDE_PATH
  include
  postgresql
)

set(SUFFIX_FOR_LIBRARY_PATH
  lib
)

#
# Look for an installation.
#
find_path(POSTGRESQL_INCLUDE_DIR
  NAMES
  libpq-fe.h
  
  PATH_SUFFIXES
  ${SUFFIX_FOR_INCLUDE_PATH}
  
  PATHS
  ${POSTGRESQL_DIR_SEARCH}

  DOC
  "The ${POSTGRESQL_INCLUDE_DIR_MESSAGE}"
)

find_library(POSTGRESQL_LIBPQ_PATH
  NAMES
  pq
  libpq
  
  PATH_SUFFIXES
  ${SUFFIX_FOR_LIBRARY_PATH}

  PATHS
  ${POSTGRESQL_DIR_SEARCH}
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PostgreSQL DEFAULT_MSG POSTGRESQL_INCLUDE_DIR POSTGRESQL_LIBPQ_PATH)

if(POSTGRESQL_INCLUDE_DIR)
  if(EXISTS "${POSTGRESQL_INCLUDE_DIR}")
    set(POSTGRESQL_INCLUDE_DIRS
      ${POSTGRESQL_INCLUDE_DIR}
      ${POSTGRESQL_INCLUDE_DIR}/libpq
    )
  endif(EXISTS "${POSTGRESQL_INCLUDE_DIR}")
endif(POSTGRESQL_INCLUDE_DIR)

set (POSTGRESQL_LIBRARIES ${POSTGRESQL_LIBPQ_PATH})

if(NOT POSTGRESQL_FOUND)
  if(NOT POSTGRESQL_FIND_QUIETLY)
    message(STATUS "POSTGRESQL was not found. ${POSTGRESQL_DIR_MESSAGE}")
  else(NOT POSTGRESQL_FIND_QUIETLY)
    if(POSTGRESQL_FIND_REQUIRED)
      message(FATAL_ERROR "POSTGRESQL was not found. ${POSTGRESQL_DIR_MESSAGE}")
    endif(POSTGRESQL_FIND_REQUIRED)
  endif(NOT POSTGRESQL_FIND_QUIETLY)
endif(NOT POSTGRESQL_FOUND)
