add_executable(basic basic/basic.c)
target_link_libraries(basic clog)

add_executable(assert extra/assert.c)
target_link_libraries(assert clog)
