find_package(Threads)
include(ExternalProject)

ExternalProject_Add(googletest
	URL https://github.com/google/googletest/archive/master.zip
	PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
	INSTALL_COMMAND ""
	BUILD_BYPRODUCTS ${CMAKE_BINARY_DIR}/unittests/gtest/src/googletest-build/googlemock/gtest/libgtest_main.a
	BUILD_BYPRODUCTS ${CMAKE_BINARY_DIR}/unittests/gtest/src/googletest-build/googlemock/gtest/libgtest.a
	BUILD_BYPRODUCTS ${CMAKE_BINARY_DIR}/unittests/gtest/src/googletest-build/googlemock/libgmock.a
)

ExternalProject_Get_Property(googletest binary_dir)

add_library(gtest UNKNOWN IMPORTED)
add_dependencies(gtest googletest)
set_target_properties(gtest PROPERTIES
	"IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
	"IMPORTED_LOCATION" ${binary_dir}/googlemock/gtest/libgtest.a
)

add_library(gmock UNKNOWN IMPORTED)
add_dependencies(gmock googletest)
set_target_properties(gmock PROPERTIES
	"IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
	"IMPORTED_LOCATION" ${binary_dir}/googlemock/libgmock.a 
)

add_library(gtest_main UNKNOWN IMPORTED)
add_dependencies(gtest_main googletest)
set_target_properties(gtest_main PROPERTIES
	"IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
	"IMPORTED_LOCATION" ${binary_dir}/googlemock/gtest/libgtest_main.a
)

include_directories(
	${CMAKE_CURRENT_BINARY_DIR}/gtest/src/googletest/googlemock/include
	${CMAKE_CURRENT_BINARY_DIR}/gtest/src/googletest/googletest/include
)
