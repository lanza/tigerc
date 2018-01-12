include_directories(${CMAKE_BINARY_DIR}/unittests/Lex)
add_executable(testLex testLex.cpp)
target_link_libraries(testLex gtest_main tigercParse tigercLex tigercParse)
add_test(NAME testLex COMMAND testLex)
