cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(core C)

set(UX_DOMAIN_HEADER
  ../../include/ux/ux.h
  ux_internal.h
)

set(UX_DOMAIN_SRC
        ux_loop.c
        ux_dispatch.c
        ux_instrument.c
        ux_order.c
        ux_barfactory.c
        ux_plugin.c
        ux_position.c
        ux_portfolio.c
)

add_library(uxdomain STATIC  ${UX_DOMAIN_HEADER} ${UX_DOMAIN_SRC})
target_link_libraries(uxdomain uxbase uxbus ${UX_EXTERNAL_LIBRARIES})
