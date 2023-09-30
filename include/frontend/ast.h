//
// Created by zc175 on 2023/9/29.
//
#pragma once
#include "frontend/lexer.h"
#include <variant>
#include <memory>
#include <optional>

class Number {
public:
    Token &token;
    explicit Number(Token & _token) : token(_token) {};
    std::variant<int, float> evaluate();
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class LVal {
public:
    Token &token;
    explicit LVal(Token &_token) : token(_token) {};
    std::variant<int, float> evaluate() const;
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class Exp;
class ParenExp {
public:
    std::unique_ptr<Exp> exp;
    explicit ParenExp(std::unique_ptr<Exp> _exp) : exp(std::move(_exp)) {};
    [[nodiscard]] std::variant<int, float> evaluate() const;
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class PrimaryExp {
public:
    std::variant<std::unique_ptr<Number>, std::unique_ptr<LVal>, std::unique_ptr<ParenExp>> exp;
    std::variant<int, float> evaluate();
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class Callee {
public:
    Token &token;
    explicit Callee(Token &_token) : token(_token) {};
    std::variant<int, float> evaluate();
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class UnaryExp;
class UnaryOp {
public:
    Token &op;
    std::unique_ptr<UnaryExp> exp;
    explicit UnaryOp(Token &_op) : op(_op) {};
    [[nodiscard]] std::variant<int, float> evaluate() const;
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class UnaryExp {
public:
    std::variant<std::unique_ptr<PrimaryExp>, std::unique_ptr<Callee>, std::unique_ptr<UnaryOp>> exp;
    std::variant<int, float> evaluate();
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class MulExp {
public:
    std::vector<Token *> ops;
    std::vector<std::unique_ptr<UnaryExp>> exps;
    std::variant<int, float> evaluate();
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class AddExp {
public:
    std::vector<Token *> ops;
    std::vector<std::unique_ptr<MulExp>> exps;
    std::variant<int, float> evaluate();
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class RelExp {
public:
    std::vector<Token *> ops;
    std::vector<std::unique_ptr<AddExp>> exps;
};

class EqExp {
public:
    std::vector<Token *> ops;
    std::vector<std::unique_ptr<RelExp>> exps;
};

class LAndExp {
public:
    std::vector<std::unique_ptr<EqExp>> exps;
};

class LOrExp {
public:
    std::vector<std::unique_ptr<LAndExp>> exps;
};

class Exp {
public:
    std::unique_ptr<AddExp> exp;
    explicit Exp(std::unique_ptr<AddExp> _exp) : exp(std::move(_exp)) {};
    [[nodiscard]] std::variant<int, float> evaluate() const;
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class ArrayUnit {
public:
    std::vector<int> indices;
    std::variant<int, float> value;
};

class VarDef {
public:
    Token &ident;
    std::unique_ptr<Exp> exp;
    std::vector<std::unique_ptr<Exp>> dims;
    std::vector<ArrayUnit> array_units;
    explicit VarDef(Token &_ident) : ident(_ident) {};
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class VarDecl {
public:
    Token &btype;
    bool is_const;
    std::vector<std::unique_ptr<VarDef>> defs;
    explicit VarDecl(Token &_btype, bool _is_const) : btype(_btype), is_const(_is_const){};
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class CompUnit {
public:
    std::vector<std::variant<std::unique_ptr<VarDecl>>> decls;
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};
