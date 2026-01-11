cmake_minimum_required(VERSION 3.10)
project(blink)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

add_compile_definitions(_GNU_SOURCE)
add_compile_definitions(CONFIG_VERSION=\"Blink-0.1\")

add_executable(blink
    core/runtime/BlinkRuntime.cpp
    core/runtime/ViewRegistry.cpp
    core/js/quickjs/quickjs.c
    core/js/quickjs/libunicode.c
    core/js/quickjs/libregexp.c
    core/js/quickjs/cutils.c
    core/js/quickjs/dtoa.c
)

target_link_libraries(blink m)

