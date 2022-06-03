#
#   MIT License
#
#   Copyright (c) 2022 Michael Rolnik
#
#   Permission is hereby granted, free of charge, to any person obtaining a copy
#   of this software and associated documentation files (the "Software"), to deal
#   in the Software without restriction, including without limitation the rights
#   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#   copies of the Software, and to permit persons to whom the Software is
#   furnished to do so, subject to the following conditions:
#
#   The above copyright notice and this permission notice shall be included in all
#   copies or substantial portions of the Software.
#
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#   SOFTWARE.
#

cmake_minimum_required(VERSION 3.16)

project(referee)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/submodules/cmake-scripts")
include(code-coverage)
include(formatting)

add_compile_options(-Wno-multichar)
add_compile_options(-g -ggdb)
add_compile_options(-std=c++20)
add_compile_options(-O0)

enable_testing()

find_package(antlr4-runtime   REQUIRED)
find_package(antlr4-generator REQUIRED)
find_package(LLVM             REQUIRED CONFIG)

add_code_coverage()

set(ANTLR4_JAR_LOCATION /opt/homebrew/Cellar/antlr/4.10.1/antlr-4.10.1-complete.jar)

antlr4_generate(
    referee_parser
    ${CMAKE_CURRENT_SOURCE_DIR}/core/referee.g4
    BOTH
    TRUE
    TRUE
    "referee"
)
include_directories(
    .
    core
    ${ANTLR4_INCLUDE_DIR_referee_parser}
    ${ANTLR4_INCLUDE_DIR}
    ${PROJECT_BINARY_DIR}
    ${ANTLR4_INCLUDE_DIR}
    ${LLVM_INCLUDE_DIRS}
    submodules/rapidcsv/src
)

include_directories(
    /opt/homebrew/Cellar/googletest/1.11.0/include
    /opt/homebrew/Cellar/antlr4-cpp-runtime/4.10.1/include/
)

link_directories(
    /opt/homebrew/Cellar/googletest/1.11.0/lib
    /opt/homebrew/Cellar/antlr4-cpp-runtime/4.10.1/lib
)

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})
llvm_map_components_to_libnames(llvm_libs support core irreader)

file(GLOB_RECURSE ALL_CODE_FILES
    ${PROJECT_SOURCE_DIR}/core/*.[ch]pp
    ${PROJECT_SOURCE_DIR}/core/*.[ch]
    ${PROJECT_SOURCE_DIR}/rdb/*.[h]pp
    ${PROJECT_SOURCE_DIR}/rdb/*.[h]
)

clang_format(format ${ALL_CODE_FILES})
message( ${ALL_CODE_FILES})

add_library(
    core
    core/visitors/compile.cpp
    core/visitors/canonic.cpp
    core/visitors/negated.cpp
    core/visitors/typecalc.cpp
    core/visitors/printer.cpp
    core/visitors/rewrite.cpp
    core/visitors/csvHeaders.cpp
    core/antlr2ast.cpp
    core/syntax.cpp
    core/strings.cpp
    core/module.cpp
    core/utils.cpp
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
    ${llvm_libs}
)

add_executable(
    tests
    test/main.cpp
    test/strings.cpp
    test/canonic.cpp)

target_link_libraries(
    tests
    core
    gtest
    gtest_main
    pthread)
add_test(tests tests)

target_code_coverage(tests)
target_code_coverage(referee)


add_executable(
    rdb
    rdb/database.cpp
)
