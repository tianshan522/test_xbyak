cmake_minimum_required(VERSION 3.10)

project(xbyak_test)

add_subdirectory(thirdparty/xbyak)

add_executable(xbyak_test
    test_main.cpp
)

target_link_libraries(xbyak_test PRIVATE xbyak)