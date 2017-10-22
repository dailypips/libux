cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(core C)

set(UX_BUS_HEADER
  bar_generator.h
  event_queue.h
)

set(UX_BUS_SRC
        event.c
        event_queue.c
        event_bus.c
        context.c
        dispatch.c
        bar_generator.c
        events.c
        dispatch_market.c
)

add_library(uxbus STATIC  ${UX_BUS_HEADER} ${UX_BUS_SRC})
target_link_libraries(uxbus uxbase ${UX_EXTERNAL_LIBRARIES})
