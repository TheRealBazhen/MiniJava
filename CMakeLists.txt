cmake_minimum_required(VERSION 3.10)

project(MiniJavaCompiler)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

BISON_TARGET(
    Parser
    parser/parser.y
    ${MiniJavaCompiler_SOURCE_DIR}/parser/parser.cpp
    DEFINES_FILE ${MiniJavaCompiler_SOURCE_DIR}/parser/parser.h
)

FLEX_TARGET(
    Scanner
    parser/scanner.l
    ${MiniJavaCompiler_SOURCE_DIR}/parser/scanner.cpp
)

ADD_FLEX_BISON_DEPENDENCY(Scanner Parser)

add_executable(
    MiniJavaCompiler
    main.cpp
    parser/driver.cpp
    ${BISON_Parser_OUTPUTS}
    ${FLEX_Scanner_OUTPUTS}
)
