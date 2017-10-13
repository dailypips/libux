cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(core C)

if(LIBUX_ENABLE_SHARED_BUILD)
  add_definitions(-DUX_BUILD_SHARED -DUX_BUILD_LIBRARY -DBUILDING_UX_SHARED)
  set(LIBUX_BUILD_TYPE SHARED)
else()
  add_definitions(-DUX_BUILD_LIBRARY)
  set(LIBUX_BUILD_TYPE STATIC)
endif()

if(LIBUX_ENABLE_AMALG)
  set(UX_CORE_HEADER)
  set(UX_CORE_SRC ux_amalg.c)
else()
set(UX_CORE_HEADER
  ../../include/ux/ux.h
  ux_internal.h
)

set(UX_CORE_SRC
        ux_datetime.c
        ux_event.c
        ux_events.c
        ux_mem.c
        ux_mpscq.c
        ux_spscq.c
        ux_queue.c
        ux_bus.c
        ux_loop.c
        ux_dispatch.c
        ux_instrument.c
        ux_order.c
        ux_barfactory.c
        ux_data_manager.c
        ux_plugin.c
)
endif()

#if(WIN32)
#  list(APPEND UX_CORE_SRC log_windows.c time_windows.c string_util_windows.c string_windows.c)
#else(WIN32)
#  list(APPEND UX_CORE_SRC log_posix.c time_posix.c string_posix.c)
#endif()

add_library(uxcore ${LIBUX_BUILD_TYPE}  ${UX_CORE_HEADER} ${UX_CORE_SRC})
target_link_libraries(uxcore cpcore ${UX_EXTERNAL_LIBRARIES})
