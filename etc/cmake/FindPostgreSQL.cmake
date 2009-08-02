
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

set(SUFFIX_FOR_INCLUDE_PATH
  include
  postgresql
)

set(SUFFIX_FOR_LIBRARY_PATH
  lib
)

find_path(POSTGRESQL_INCLUDE_DIR
  NAMES
  libpq-fe.h
  
  PATH_SUFFIXES
  ${SUFFIX_FOR_INCLUDE_PATH}
  
  PATHS
  ${POSTGRESQL_DIR_SEARCH}
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

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PostgreSQL DEFAULT_MSG POSTGRESQL_INCLUDE_DIR POSTGRESQL_LIBPQ_PATH)

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
