cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

include_directories(base)
include_directories(bus)
include_directories(domain)
include_directories(manager)
add_subdirectory(base)
#add_subdirectory(util)
add_subdirectory(bus)
#add_subdirectory(domain)
#add_subdirectory(manager)

