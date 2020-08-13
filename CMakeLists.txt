cmake_minimum_required(VERSION 3.10)

project(MiniJavaCompiler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG")

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

BISON_TARGET(
        Parser
        parser/parser.y
        ${MiniJavaCompiler_SOURCE_DIR}/parser/parser.cpp
        DEFINES_FILE ${MiniJavaCompiler_SOURCE_DIR}/parser/parser.h
        VERBOSE
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
        program/arguments/arguments.cpp
        program/call/call.cpp
        program/declaration/declaration.cpp
        program/expression/expression.cpp
        program/lvalue/lvalue.cpp
        program/program/program.cpp
        program/statement/statement.cpp
        program/types/types.cpp
        visitors/function_caller/function_caller.cpp
        visitors/tree_printer/tree_printer.cpp
        visitors/interpreter/interpreter.cpp
        visitors/symbol_tree_builder/symbol_tree_builder.cpp
        symbol_tree/symbol_layer.cpp
        symbol_tree/symbol_tree.cpp
        types/type.cpp
        function_mechanism/frame/frame.cpp
        function_mechanism/index_table/index_table.cpp
        class_storage/class_storage.cpp
        ${BISON_Parser_OUTPUTS}
        ${FLEX_Scanner_OUTPUTS}
)

target_include_directories(MiniJavaCompiler PRIVATE ${MiniJavaCompiler_SOURCE_DIR})
