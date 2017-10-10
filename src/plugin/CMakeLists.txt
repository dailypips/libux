cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(core C)

set(CP_VERSION_MAJOR 1)
set(CP_VERSION_MINOR 0)
set(CP_VERSION_PATCH 0)
# create full version string
set(CP_VERSION "${CP_VERSION_MAJOR}.${CP_VERSION_MINOR}.${CP_VERSION_PATCH}")
# definitions
add_definitions(-DCP_VERSION_MAJOR=${CP_VERSION_MAJOR})
add_definitions(-DCP_VERSION_MINOR=${CP_VERSION_MINOR})
add_definitions(-DCP_VERSION_PATCH=${CP_VERSION_PATCH})
add_definitions(-DCP_VERSION="${CP_VERSION}")
add_definitions(-DCP_THREADS)
add_definitions(-DCP_HOST="${CMAKE_SYSTEM_NAME}")
add_definitions(-DCP_FNAMESEP_CHAR='/')
add_definitions(-DCP_SHREXT="dylib")
#

find_package(EXPAT REQUIRED)
if(EXPAT_FOUND)
  include_directories(${EXPAT_INCLUDE_DIRS})
endif()

if(LIBUX_ENABLE_SHARED_BUILD)
  add_definitions(-DUX_BUILD_SHARED -DUX_BUILD_LIBRARY)
  set(LIBUX_BUILD_TYPE SHARED)
else()
  add_definitions(-DUX_BUILD_LIBRARY)
  set(LIBUX_BUILD_TYPE STATIC)
endif()

if(NOT WIN32)
  add_definitions(-DDLOPEN_POSIX)
  #add_definitions(-DDLOPEN_LIBTOOL)
endif()

set(UX_CP_HEADER
      cp.h
      cp_def.h
      cp_hash.h
      cp_list.h
      cp_internal.h
      cp_nls.h
      cp_thread.h
      cp_util.h
)

set(UX_CP_SRC
        cp.c
        cp_context.c
        cp_hash.c
        cp_list.c
        cp_logging.c
        cp_descriptor.c
        cp_info.c
        cp_loader.c
        cp_control.c
        cp_scan.c
        cp_symbol.c
        cp_serial.c
        cp_util.c
)

if(WIN32)
  list(APPEND UX_CP_SRC thread_windows.c)
else(WIN32)
  list(APPEND UX_CP_SRC thread_posix.c)
endif()

add_library(cpcore ${LIBUX_BUILD_TYPE}  ${UX_CP_HEADER} ${UX_CP_SRC})
target_link_libraries(cpcore  ${EXPAT_LIBRARIES})
