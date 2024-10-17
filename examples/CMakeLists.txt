# Basic
add_executable(basic basic/basic.c)
target_link_libraries(basic clog)

add_executable(newline basic/newline.c)
target_link_libraries(newline clog)

add_executable(callback basic/callback.c)
target_link_libraries(callback clog)

# Extra
add_executable(assert extra/assert.c)
target_link_libraries(assert clog)
