set(tigerc_link_components support)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  set(debug_flag "--debug")
endif()

file(MAKE_DIRECTORY 
  ${CMAKE_BINARY_DIR}/lib/Parse
  ${CMAKE_BINARY_DIR}/include/tigerc/Parse
  )
add_custom_target(Parser ALL
  COMMAND /usr/local/bin/bison ${CMAKE_SOURCE_DIR}/lib/Parse/Parser.ypp -o ${CMAKE_BINARY_DIR}/lib/Parse/Parser.cpp --defines=${CMAKE_BINARY_DIR}/include/tigerc/Parse/Parser.h ${debug_flag} -v
  BYPRODUCTS ${CMAKE_BINARY_DIR}/lib/Parse/Parser.cpp ${CMAKE_BINARY_DIR}/include/tigerc/Parse/Parser.h
  SOURCES Parser.ypp
)

include_directories(${CMAKE_BINARY_DIR}/include)

add_library(tigercParse ${CMAKE_BINARY_DIR}/lib/Parse/Parser.cpp)
target_link_libraries(tigercParse tigercSupport)
