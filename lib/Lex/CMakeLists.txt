set(tigerc_link_components support)

file(MAKE_DIRECTORY 
  ${CMAKE_BINARY_DIR}/lib/Lex
  ${CMAKE_BINARY_DIR}/include/tigerc/Lex
  )

add_custom_target(Lexer ALL 
  COMMAND /usr/local/bin/flex++ -o ${CMAKE_BINARY_DIR}/lib/Lex/Lexer.cpp --header-file=${CMAKE_BINARY_DIR}/include/tigerc/Lex/Lexer.h -c++ ${CMAKE_SOURCE_DIR}/lib/Lex/Lexer.flex
  BYPRODUCTS ${CMAKE_BINARY_DIR}/lib/Lex/Lexer.cpp ${CMAKE_BINARY_DIR}/include/tigerc/Lex/Lexer.h
  SOURCES Lexer.flex 
)

include_directories(${CMAKE_BINARY_DIR}/include)
  
add_dependencies(Lexer Parser)
add_library(tigercLex ${CMAKE_BINARY_DIR}/lib/Lex/Lexer.cpp)
target_link_libraries(tigercLex tigercSupport l) 
