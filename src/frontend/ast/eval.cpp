//
// Created by zc175 on 2023/9/30.
//
#include "frontend/ast.h"
#include "frontend/env.h"
#include "utils/error.h"
#include "utils/conv.h"

std::variant<int, float> operator-(const std::variant<int, float> &left) {
    if (left.index() == 0) {
        return -std::get<int>(left);
    } else {
        return -std::get<float>(left);
    }
}

std::variant<int, float> operator+(const std::variant<int, float> &left, const std::variant<int, float> &right) {
    float float_result = 0;
    int int_result = 0;
    bool is_float = false;

    if (left.index() == 0) {
        int_result += std::get<int>(left);
    } else {
        is_float = true;
        float_result += std::get<float>(left);
    }

    if (right.index() == 0) {
        if (is_float) {
            return float_result + (float)std::get<int>(right);
        } else {
            return int_result + std::get<int>(right);
        }
    } else {
        if (is_float) {
            return float_result + std::get<float>(right);
        } else {
            return (float)int_result + std::get<float>(right);
        }
    }
}

std::variant<int, float> operator-(const std::variant<int, float> &left, const std::variant<int, float> &right) {
    float float_result = 0;
    int int_result = 0;
    bool is_float = false;

    if (left.index() == 0) {
        int_result += std::get<int>(left);
    } else {
        is_float = true;
        float_result += std::get<float>(left);
    }

    if (right.index() == 0) {
        if (is_float) {
            return float_result - (float)std::get<int>(right);
        } else {
            return int_result - std::get<int>(right);
        }
    } else {
        if (is_float) {
            return float_result - std::get<float>(right);
        } else {
            return (float)int_result - std::get<float>(right);
        }
    }
}

std::variant<int, float> operator*(const std::variant<int, float> &left, const std::variant<int, float> &right) {
    float float_result = 0;
    int int_result = 0;
    bool is_float = false;

    if (left.index() == 0) {
        int_result += std::get<int>(left);
    } else {
        is_float = true;
        float_result += std::get<float>(left);
    }

    if (right.index() == 0) {
        if (is_float) {
            return float_result * (float)std::get<int>(right);
        } else {
            return int_result * std::get<int>(right);
        }
    } else {
        if (is_float) {
            return float_result * std::get<float>(right);
        } else {
            return (float)int_result * std::get<float>(right);
        }
    }
}

std::variant<int, float> operator/(const std::variant<int, float> &left, const std::variant<int, float> &right) {
    float float_result = 0;
    int int_result = 0;
    bool is_float = false;

    if (left.index() == 0) {
        int_result += std::get<int>(left);
    } else {
        is_float = true;
        float_result += std::get<float>(left);
    }

    if (right.index() == 0) {
        if (is_float) {
            return float_result / (float)std::get<int>(right);
        } else {
            return int_result / std::get<int>(right);
        }
    } else {
        if (is_float) {
            return float_result / std::get<float>(right);
        } else {
            return (float)int_result / std::get<float>(right);
        }
    }
}

std::variant<int, float> operator%(const std::variant<int, float> &left, const std::variant<int, float> &right) {
    int int_result = 0;

    if (left.index() == 0) {
        int_result += std::get<int>(left);
    } else {
        critical_error("Mod operation for non-int value");
    }

    if (right.index() == 0) {
        return int_result % std::get<int>(right);
    } else {
        critical_error("Mod operation for non-int value");
    }
    return 1;
}

std::variant<int, float> Exp::evaluate() const {
    return exp->evaluate();
}

std::variant<int, float> AddExp::evaluate() {
    auto result = exps.at(0)->evaluate();
    for (int i = 0; i < ops.size(); i++) {
        if (ops.at(i)->expect({TK_OP_ADD})) {
            result = result + exps.at(i + 1)->evaluate();
        } else {
            result = result - exps.at(i + 1)->evaluate();
        }
    }
    return result;
}

std::variant<int, float> MulExp::evaluate() {
    auto result = exps.at(0)->evaluate();
    for (int i = 0; i < ops.size(); i++) {
        if (ops.at(i)->expect({TK_OP_MUL})) {
            result = result * exps.at(i + 1)->evaluate();
        } else if (ops.at(i)->expect({TK_OP_DIV})) {
            result = result / exps.at(i + 1)->evaluate();
        } else {
            result = result % exps.at(i + 1)->evaluate();
        }
    }
    return result;
}

std::variant<int, float> UnaryExp::evaluate() {
    auto visitor = [&](auto &arg) {
        return arg->evaluate();
    };
    return std::visit(visitor, exp);
}

std::variant<int, float> PrimaryExp::evaluate() {
    auto visitor = [&](auto &arg) {
        return arg->evaluate();
    };
    return std::visit(visitor, exp);
}

std::variant<int, float> Callee::evaluate() {
    critical_error(token, "Function call is not allowed in a const expression.");
    return 1;
}

std::variant<int, float> UnaryOp::evaluate() const {
    if (op.expect({TK_OP_ADD})) {
        return exp->evaluate();
    } else {
        return -exp->evaluate();
    }
}

std::variant<int, float> Number::evaluate() {
    std::variant<int, float> result;
    if (token.expect({TK_CONST_INT})) {
        result = ttoi(token);
    } else {
        result = ttof(token);
    }
    return result;
}

std::variant<int, float> LVal::evaluate() const {
    return current_env->get_symbol(token.raw).value;
}

std::variant<int, float> ParenExp::evaluate() const {
    return exp->evaluate();
}
