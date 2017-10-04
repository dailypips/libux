cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(core C)

set(UX_CORE_HEADER

)

set(UX_CORE_SRC
        ux_datetime.c
        ux_event.c
        ux_mem.c
        ux_mpscq.c
        ux_spscq.c
        ux_queue.c
        ux_bus.c
        ux_loop.c
        ux_dispatch.c
)

#if(WIN32)
#  list(APPEND UX_CORE_SRC log_windows.c time_windows.c string_util_windows.c string_windows.c)
#else(WIN32)
#  list(APPEND UX_CORE_SRC log_posix.c time_posix.c string_posix.c)
#endif()

add_library(uxcore SHARED  ${UX_CORE_HEADER} ${UX_CORE_SRC})
target_link_libraries(uxcore ${UX_EXTERNAL_LIBRARIES})
