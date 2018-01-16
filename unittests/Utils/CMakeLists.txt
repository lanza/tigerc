include_directories(${CMAKE_BINARY_DIR}/include)
add_library(testingUtils testingUtils.cpp)
target_link_libraries(testingUtils gtest gmock tigercLex tigercParse tigercSupport)
