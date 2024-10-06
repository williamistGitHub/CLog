add_executable(basic basic/basic.c)
add_executable(assert extra/assert.c)
target_link_libraries(basic clog)
target_link_libraries(assert clog)
