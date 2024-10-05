cmake_minimum_required(VERSION 3.24)
project(
    CLog
    VERSION 0.1.0
    DESCRIPTION "Simple C89 logging library."
    LANGUAGES C
)

add_library(clog src/clog.c)

target_include_directories(clog PUBLIC include)

set_target_properties(
    clog PROPERTIES

    C_STANDARD 90
    COMPILE_WARNING_AS_ERROR TRUE
)

# the fact i have to specify command line arguments here (and compiler specific
# ones at that!), defeating the point of using cmake at all, annoys me.
if(MSVC)
    target_compile_options(clog PRIVATE /W4 /WX)
    target_compile_definitions(clog PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(clog PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# examples
option(BUILD_EXAMPLES "Build CLog examples" OFF)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
