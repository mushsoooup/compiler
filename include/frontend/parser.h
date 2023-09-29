//
// Created by zc175 on 2023/9/29.
//
#pragma once
#include <string>
#include <utility>
#include "frontend/lexer.h"
#include "frontend/ast.h"

class Parser {
public:
    TokenList token_list;
    explicit Parser(TokenList _token_list) : token_list(std::move(_token_list)) {};
};
