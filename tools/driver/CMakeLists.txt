add_executable(tc1 tc1_main.cpp) 
add_executable(tigerc driver.cpp)

target_link_libraries(tigerc
  tigercSupport
  tigercLex
)
target_link_libraries(tc1
  tigercSupport
  tigercLex
)
