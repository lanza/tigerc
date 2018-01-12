cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(tigerc)

include_directories(
  "${CMAKE_SOURCE_DIR}/include" 
  ${CMAKE_BINARY_DIR}/lib/Lex/
  ${CMAKE_BINARY_DIR}/lib/Parse/
)

enable_testing()
add_subdirectory("${CMAKE_SOURCE_DIR}/lib")
add_subdirectory("${CMAKE_SOURCE_DIR}/tools")
add_subdirectory("${CMAKE_SOURCE_DIR}/unittests")

