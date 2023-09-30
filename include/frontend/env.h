//
// Created by zc175 on 2023/9/30.
//
#pragma once
#include <unordered_map>
#include <memory>
#include <variant>
#include <vector>

class Symbol{
public:
    enum SYMBOL_TYPE{
        SYMBOL_FUNC_INT,
        SYMBOL_FUNC_FLOAT,
        SYMBOL_INT,
        SYMBOL_FLOAT,
        SYMBOL_INT_CONST,
        SYMBOL_FLOAT_CONST,
    };
    SYMBOL_TYPE type;
    std::variant<int, float> value;
};

class Env {
public:
    Env *outer;
    std::unordered_map<std::string, Symbol> symbols;
    std::vector<Env *> children;
    void add_symbol(const std::string&, Symbol);
    Symbol get_symbol(const std::string&);
};

extern Env *global_env;
extern Env *current_env;
