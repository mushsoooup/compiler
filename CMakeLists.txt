cmake_minimum_required(VERSION 3.26)
project(compiler)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS  "-g")
include_directories(include)

add_executable(compiler src/main.cpp
        src/utils/conv.cpp
        src/utils/error.cpp
        src/frontend/lexer/lexer.cpp
        src/frontend/lexer/token.cpp
        src/frontend/parser/parser.cpp
        src/frontend/parser/print.cpp
        src/frontend/parser/eval.cpp
)
