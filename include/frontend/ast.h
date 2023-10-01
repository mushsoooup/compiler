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
    [[nodiscard]] std::variant<int, float> evaluate() const;
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class Exp;
class ParenExp {
public:
    std::shared_ptr<Exp> exp;
    explicit ParenExp(std::shared_ptr<Exp> _exp) : exp(std::move(_exp)) {};
    [[nodiscard]] std::variant<int, float> evaluate() const;
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class PrimaryExp {
public:
    std::variant<std::shared_ptr<Number>, std::shared_ptr<LVal>, std::shared_ptr<ParenExp>> exp;
    std::variant<int, float> evaluate();
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class Callee {
public:
    Token &token;
    std::vector<std::shared_ptr<Exp>> params;
    explicit Callee(Token &_token) : token(_token) {};
    std::variant<int, float> evaluate();
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class UnaryExp;
class UnaryOp {
public:
    Token &op;
    std::shared_ptr<UnaryExp> exp;
    explicit UnaryOp(Token &_op) : op(_op) {};
    [[nodiscard]] std::variant<int, float> evaluate() const;
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class UnaryExp {
public:
    std::variant<std::shared_ptr<PrimaryExp>, std::shared_ptr<Callee>, std::shared_ptr<UnaryOp>> exp;
    std::variant<int, float> evaluate();
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class MulExp {
public:
    std::vector<Token *> ops;
    std::vector<std::shared_ptr<UnaryExp>> exps;
    std::variant<int, float> evaluate();
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class AddExp {
public:
    std::vector<Token *> ops;
    std::vector<std::shared_ptr<MulExp>> exps;
    std::variant<int, float> evaluate();
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class RelExp {
public:
    std::vector<Token *> ops;
    std::vector<std::shared_ptr<AddExp>> exps;
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class EqExp {
public:
    std::vector<Token *> ops;
    std::vector<std::shared_ptr<RelExp>> exps;
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class LAndExp {
public:
    std::vector<std::shared_ptr<EqExp>> exps;
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class LOrExp {
public:
    std::vector<std::shared_ptr<LAndExp>> exps;
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class Exp {
public:
    std::shared_ptr<AddExp> exp;
    explicit Exp(std::shared_ptr<AddExp> _exp) : exp(std::move(_exp)) {};
    [[nodiscard]] std::variant<int, float> evaluate() const;
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class VarDef {
public:
    Token &ident;
    std::shared_ptr<Exp> exp;
    std::vector<std::shared_ptr<Exp>> dims;
    explicit VarDef(Token &_ident) : ident(_ident) {};
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class VarDecl {
public:
    Token &btype;
    bool is_const;
    std::vector<std::shared_ptr<VarDef>> defs;
    explicit VarDecl(Token &_btype, bool _is_const) : btype(_btype), is_const(_is_const){};
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class FuncFParam {
public:
    Token &btype;
    Token &ident;
    explicit FuncFParam(Token &_btype, Token &_ident) : btype(_btype), ident(_ident) {};
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class Cond {
public:
    std::shared_ptr<LOrExp> exp;
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class AssignStmt {
public:
    std::shared_ptr<LVal> lval;
    std::shared_ptr<Exp> exp;
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class Stmt;
class IfStmt {
public:
    std::shared_ptr<Cond> cond;
    std::shared_ptr<Stmt> body;
    std::shared_ptr<Stmt> else_body;
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class WhileStmt {
public:
    std::shared_ptr<Cond> cond;
    std::shared_ptr<Stmt> body;
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class CtrlStmt {
public:
    // true for break, false for continue;
    bool type;
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class ReturnStmt {
public:
    std::shared_ptr<Exp> exp;
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class Block {
public:
    std::vector<std::variant<std::shared_ptr<VarDecl>, std::shared_ptr<Stmt>>> items;
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class FuncDef {
public:
    Token &functype;
    Token &ident;
    std::vector<std::shared_ptr<FuncFParam>> params;
    std::shared_ptr<Block> block;
    explicit FuncDef(Token &_functype, Token &_ident) : functype(_functype), ident(_ident) {};
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class Stmt {
public:
    std::variant<std::shared_ptr<AssignStmt>, std::shared_ptr<Exp>, std::shared_ptr<Block>,  std::shared_ptr<IfStmt>,
            std::shared_ptr<WhileStmt>, std::shared_ptr<CtrlStmt>, std::shared_ptr<ReturnStmt>> stmt;
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class CompUnit {
public:
    std::vector<std::variant<std::shared_ptr<VarDecl>, std::shared_ptr<FuncDef>>> decls;
    void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};
