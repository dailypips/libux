cmake_minimum_required(VERSION 3.1)

function(add_unit_test target)
  string(REPLACE ".c" "" exe_name ${target})
    add_executable(${exe_name} ${target})
    target_link_libraries(${exe_name} uxcore ${QUANT_EXTERNAL_LIBRARIES})
    add_test(NAME ${exe_name} COMMAND ${exe_name})

endfunction()

include_directories("../src")

file(GLOB_RECURSE TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} 
          ${CMAKE_CURRENT_SOURCE_DIR}/test_*.c)

foreach(item  ${TEST_SRCS})
  add_unit_test(${item})
endforeach()

