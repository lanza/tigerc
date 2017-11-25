cmake_minimum_required(VERSION 3.8)

project(tigerc)

set(tigerc_main_dir "/Users/lanza/Projects/tigerc")

include_directories("${tigerc_main_dir}/include")

enable_testing()
add_subdirectory("${tigerc_main_dir}/lib")
add_subdirectory("${tigerc_main_dir}/tools")
add_subdirectory("${tigerc_main_dir}/unittests")

