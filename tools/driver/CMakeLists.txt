add_executable(tigerc
  tc1_main.c
)

target_link_libraries(tigerc
  tigercSupport
  tigercLex
  )
