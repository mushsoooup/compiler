//
// Created by zc175 on 2023/9/30.
//
#include "frontend/env.h"

Env *global_env = new Env;
Env *current_env = global_env;

void Env::add_symbol(const std::string& name, const Symbol symbol) {
    symbols[name] = symbol;
}

Symbol Env::get_symbol(const std::string &name) {
    return symbols[name];
}
