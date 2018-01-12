set(tigerc_link_components support)

find_package(FLEX)
set(flex_src tiger.l)

flex_target(
  lex 
    ${flex_src} ${CMAKE_CURRENT_BINARY_DIR}/lex.c 
    COMPILE_FLAGS --header-file=${CMAKE_BINARY_DIR}/lib/Lex/lex.h
)

include_directories(${CMAKE_CURRENT_BINARY_DIR} )
  
add_library(tigercLex ${FLEX_lex_OUTPUTS} ${BISON_OUTPUT_HEADER})
target_link_libraries(tigercLex tigercSupport l) 
