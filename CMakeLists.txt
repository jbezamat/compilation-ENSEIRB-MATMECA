cmake_minimum_required(VERSION 2.8)
project(AS)
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)
find_package(Doxygen)

add_definitions(-std=gnu99 -pedantic -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wno-sign-compare -Wno-unused-function -Wno-unused-variable)
#TODO ADD -Werror

# Useful paths
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(TESTS_PATH ${EXECUTABLE_OUTPUT_PATH}/tests)
include_directories(${PROJECT_SOURCE_DIR}/include/ ${PROJECT_BINARY_DIR}/src/)

enable_testing()

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE "DEBUG")
else("${CMAKE_BUILD_TYPE}" STREQUAL "")
  string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)
endif("${CMAKE_BUILD_TYPE}" STREQUAL "")

message(STATUS "Mode : ${CMAKE_BUILD_TYPE}")

add_subdirectory(src)
add_subdirectory(test)
