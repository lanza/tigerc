set(tigerc_link_components support)

add_library(tigercInterpreter Interpreter.cpp)
target_link_libraries(tigercInterpreter tigercSupport tigercParse tigercLex) 
