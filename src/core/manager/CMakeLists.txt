cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(core C)

set(UX_MANAGER_HEADER
)

set(UX_MANAGER_SRC
)

add_library(uxmanager STATIC  ${UX_MANAGER_HEADER} ${UX_MANAGER_SRC})
target_link_libraries(uxmanager uxbase uxbus uxdomain ${UX_EXTERNAL_LIBRARIES})
