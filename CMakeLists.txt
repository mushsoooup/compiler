cmake_minimum_required(VERSION 3.26)
project(compiler)

set(CMAKE_CXX_STANDARD 23)

include_directories(include)

add_executable(compiler src/main.cpp
        src/utils/conv.cpp
        include/utils/conv.h
        src/frontend/lexer/lexer.cpp
        include/frontend/lexer.h
        src/frontend/lexer/token.cpp
        src/utils/error.cpp
        include/utils/error.h
        include/frontend/parser.h
        include/frontend/ast.h
        src/frontend/ast/parser.cpp
        src/frontend/ast/print.cpp
        src/frontend/ast/eval.cpp
        include/frontend/env.h
        src/frontend/ast/env.cpp
)
