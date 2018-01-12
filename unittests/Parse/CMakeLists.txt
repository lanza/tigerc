include_directories(${CMAKE_BINARY_DIR}/unittests/Lex)
add_executable(testParse testParse.cpp)
target_link_libraries(testParse  tigercParse gtest_main tigercLex)
add_test(NAME testParse COMMAND testParse)
