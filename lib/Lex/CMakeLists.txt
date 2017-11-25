set(tigerc_link_components support)

find_package(FLEX)
set(flex_src
  tiger.l
)

FLEX_TARGET(tiger ${flex_src} ${CMAKE_BINARY_DIR}/tiger-lex.c COMPILE_FLAGS
  "--header-file=${CMAKE_BINARY_DIR}/unittests/Lex/tiger-lex.h")
include_directories(${CMAKE_BINARY_DIR})
add_library(tigercLex
  ${FLEX_tiger_OUTPUTS}
)
target_link_libraries(tigercLex tigercSupport l)
