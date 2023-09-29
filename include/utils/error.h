//
// Created by zc175 on 2023/9/29.
//
#pragma once
#include <string>
#include <iostream>
#include "frontend/lexer.h"

void critical_error(Token &token, const char *fmt...);
void critical_error(const char *fmt...);
