cmake_minimum_required(VERSION 3.16)

project(monitor)

add_compile_options(-Wno-multichar)
add_compile_options(-g -ggdb)

enable_testing()

include_directories(
    .
    core
)

add_library(
    core
    core/canonic.cpp
    core/negated.cpp
    core/syntax.cpp
)

add_executable(
    monitor
    main.cpp
)
target_link_libraries(
    monitor
    core
)

add_executable(
    tests
    test/main.cpp
    test/canonic.cpp)

target_link_libraries(
    tests
    core
    gtest
    gtest_main
    pthread)
add_test(tests tests)
