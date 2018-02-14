#include(gtest.cmake)

include_directories(../cget/include)
link_directories(../cget/lib)

include_directories(Utils)

add_subdirectory(Utils) 
add_subdirectory(Lex)
#add_subdirectory(Parse)
