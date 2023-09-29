//
// Created by zc175 on 2023/9/28.
//
#include "utils/conv.h"
#include "utils/error.h"

float ttof(Token &token) {
    float result{};
    try {
        result = std::stof(token.raw);
    } catch (...){
        critical_error(token, "Unexpected stof failure.");
    }
    return result;
}

int ttoi(Token &token) {
    int result{};
    try {
        result = std::stoi(token.raw, nullptr, 0);
    } catch (...) {
        critical_error(token, "Unexpected stoi failure.");
    }
    return result;
}