cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(addon C)

set(UX_ADDON_HEADER
)

set(UX_ADDON_SRC
        time_bar.c
)

add_library(uxaddon SHARED  ${UX_ADDON_HEADER} ${UX_ADDON_SRC})
target_link_libraries(uxaddon uxcore  ${UX_EXTERNAL_LIBRARIES})
