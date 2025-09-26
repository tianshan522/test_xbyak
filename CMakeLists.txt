cmake_minimum_required(VERSION 3.10)

project(xbyak_test)

add_subdirectory(thirdparty/xbyak)

add_executable(xbyak_test
    test_xbyak.cpp
)

target_link_libraries(xbyak_test PRIVATE xbyak)

add_executable(fp16_test
    avx512_fp16_test.cpp
)

target_link_libraries(fp16_test PRIVATE xbyak)
