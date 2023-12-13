//
// Created by zc175 on 2023/9/29.
//
#pragma once
#include "frontend/lexer.h"
#include <iostream>
#include <string>

void critical_error(Token &token, const char *fmt...);
void critical_error(const char *fmt...);
