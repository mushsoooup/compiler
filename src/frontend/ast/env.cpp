//
// Created by zc175 on 2023/9/30.
//
#include "frontend/env.h"

void Env::add_symbol(const std::string& name, const Symbol symbol) {
    symbols[name] = symbol;
}

Symbol Env::get_symbol(const std::string &name) {
    return symbols[name];
}
