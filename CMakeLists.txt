cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 17)

project(tigerc CXX)

include_directories(
  ${CMAKE_SOURCE_DIR}/include 
)

enable_testing()
add_subdirectory("${CMAKE_SOURCE_DIR}/lib")
add_subdirectory("${CMAKE_SOURCE_DIR}/tools")
add_subdirectory("${CMAKE_SOURCE_DIR}/unittests")
