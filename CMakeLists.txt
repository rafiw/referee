cmake_minimum_required(VERSION 3.16)

project(referee)

add_compile_options(-Wno-multichar)
add_compile_options(-g -ggdb)
add_compile_options(-std=c++17)

enable_testing()

find_package(antlr4-runtime   REQUIRED)
find_package(antlr4-generator REQUIRED)
set(ANTLR4_JAR_LOCATION /opt/homebrew/Cellar/antlr/4.10.1/antlr-4.10.1-complete.jar)

antlr4_generate(
    referee_parser
    ${CMAKE_CURRENT_SOURCE_DIR}/core/referee.g4
    BOTH
    TRUE
    TRUE
    "referee"
)
message("---- ${ANTLR4_INCLUDE_DIR_referee_parser}")
message("---- ${ANTLR4_SRC_FILES_referee_parser}")
include_directories(
    .
    core
    ${ANTLR4_INCLUDE_DIR_referee_parser}
    ${ANTLR4_INCLUDE_DIR}
    ${PROJECT_BINARY_DIR}
    ${ANTLR4_INCLUDE_DIR}
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
    ${ANTLR4_SRC_FILES_referee_parser}
)

add_executable(
    referee
    main.cpp
)
add_dependencies(
    referee
    antlr4_shared
)

target_link_libraries(
    referee
    core
    antlr4_shared
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
