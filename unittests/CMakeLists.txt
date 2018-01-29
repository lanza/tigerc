include(gtest.cmake)

include_directories(${CMAKE_BINARY_DIR}/googletest-src/googlemock/include)
include_directories(${CMAKE_BINARY_DIR}/googletest-src/googletest/include)
include_directories(Utils)

add_subdirectory(Utils) 
add_subdirectory(Lex)
#add_subdirectory(Parse)
