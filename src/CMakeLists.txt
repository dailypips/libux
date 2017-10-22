cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

include_directories(core core/base core/bus)

#include_directories(plugin)
#add_subdirectory(plugin)
add_subdirectory(core)
add_subdirectory(addon)

