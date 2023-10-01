//
// Created by zc175 on 2023/10/1
//
#include "frontend/visitor.h"
#include "frontend/env.h"

class Visitor {
public:
    Env *current;
    Visitor() : current(new Env) {};
};