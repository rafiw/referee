cmake_minimum_required(VERSION 3.16)

project(monitor)
include(ExternalAntlr4Cpp)


add_compile_options(-Wno-multichar)
add_compile_options(-g -ggdb)
add_compile_options(-std=c++17)

enable_testing()

include_directories(
    .
    core
)

include_directories(
    /opt/homebrew/Cellar/googletest/1.11.0/include
)

link_directories(
    /opt/homebrew/Cellar/googletest/1.11.0/lib
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
