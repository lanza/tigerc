include_directories(${CMAKE_BINARY_DIR}/include)
add_executable(testParse testParse.cpp)
target_link_libraries(testParse tigercParse gtest_main tigercLex testingUtils)
add_test(NAME testParse COMMAND testParse)
