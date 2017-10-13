cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

add_subdirectory(plugin)

include_directories(common)
include_directories(plugin)
add_subdirectory(core)
#add_subdirectory(addon)

