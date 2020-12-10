project(monitor)

add_compile_options(-Wno-multichar)

add_executable(
    monitor
    src/main.cpp
    src/canonic.cpp
    src/negated.cpp
    src/syntax.cpp
)
