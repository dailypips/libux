cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(core C)

set(UX_BASE_HEADER
  ../../../include/ux/base/ux_atomic.h
  ../../../include/ux/base/ux_common.h
  ../../../include/ux/base/ux_currency.h
  ../../../include/ux/base/ux_datetime.h
  ../../../include/ux/base/ux_idarray.h
  ../../../include/ux/base/ux_memory.h
  atomic.h
  hash.h
  mpscq.h
  spscq.h
)

set(UX_BASE_SRC
        currency.c
        datetime.c
        hash.c
        memory.c
        mpscq.c
        spscq.c
)

add_library(uxbase STATIC ${UX_BASE_HEADER} ${UX_BASE_SRC})
target_link_libraries(uxbase ${UX_EXTERNAL_LIBRARIES})
