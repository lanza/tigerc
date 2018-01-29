include_directories(${CMAKE_BINARY_DIR}/include)
add_executable(testLex testLex.cpp)
target_link_libraries(testLex gtest_main testingUtils tigercParse tigercLex tigercParse tigercInterpreter)
add_test(NAME testLex COMMAND testLex)
