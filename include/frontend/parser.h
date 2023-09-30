//
// Created by zc175 on 2023/9/29.
//
#pragma once
#include <string>
#include <utility>
#include "frontend/lexer.h"
#include "frontend/ast.h"
#include "frontend/env.h"

class Parser {
public:
    TokenList token_list;
    std::unique_ptr<CompUnit> comp_unit;
    explicit Parser(TokenList _token_list) : token_list(std::move(_token_list)) {};
    void parse();
    void output_ast(std::ostream &) const;
private:
    Token &read();
    Token &peek(size_t pos = 0);
    std::unique_ptr<CompUnit> ParseCompUnit();
    std::unique_ptr<VarDecl> ParseVarDecl();
    std::unique_ptr<VarDef> ParseVarDef();
    std::unique_ptr<Exp> ParseExp();
    std::unique_ptr<MulExp> ParseMulExp();
    std::unique_ptr<AddExp> ParseAddExp();
    std::unique_ptr<RelExp> ParseRelExp();
    std::unique_ptr<EqExp> ParseEqExp();
    std::unique_ptr<LAndExp> ParseLAndExp();
    std::unique_ptr<LOrExp> ParseLOrExp();
    std::unique_ptr<UnaryExp> ParseUnaryExp();
    std::unique_ptr<PrimaryExp> ParsePrimaryExp();
    std::unique_ptr<Callee> ParseCallee();
    std::unique_ptr<UnaryOp> ParseUnaryOp();
    std::unique_ptr<Number> ParseNumber();
    std::unique_ptr<LVal> ParseLVal();
    std::unique_ptr<ParenExp> ParseParenExp();
};
