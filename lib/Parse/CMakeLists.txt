set(tigerc_link_components support)

find_package(BISON)
set(bison_src tiger.y)

message("cbt: " ${CMAKE_BUILD_TYPE})
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  message("true")
  set(debug_flag "--debug")
endif()

message("debug flag: " ${debug_flag})
bison_target(parse ${bison_src} ${CMAKE_CURRENT_BINARY_DIR}/parse.c COMPILE_FLAGS "-d ${debug_flag} -v")

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(tigercParse ${BISON_parse_OUTPUTS})
target_link_libraries(tigercParse tigercSupport)
