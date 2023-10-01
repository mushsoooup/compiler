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
    std::shared_ptr<CompUnit> comp_unit;
    explicit Parser(TokenList _token_list) : token_list(std::move(_token_list)) {};
    void parse();
    void output_ast(std::ostream &) const;
private:
    Token &read();
    Token &peek(size_t pos = 0);
    std::shared_ptr<CompUnit> ParseCompUnit();
    std::shared_ptr<VarDecl> ParseVarDecl();
    std::shared_ptr<VarDef> ParseVarDef();
    std::shared_ptr<Exp> ParseExp();
    std::shared_ptr<MulExp> ParseMulExp();
    std::shared_ptr<AddExp> ParseAddExp();
    std::shared_ptr<RelExp> ParseRelExp();
    std::shared_ptr<EqExp> ParseEqExp();
    std::shared_ptr<LAndExp> ParseLAndExp();
    std::shared_ptr<LOrExp> ParseLOrExp();
    std::shared_ptr<UnaryExp> ParseUnaryExp();
    std::shared_ptr<PrimaryExp> ParsePrimaryExp();
    std::shared_ptr<Callee> ParseCallee();
    std::shared_ptr<UnaryOp> ParseUnaryOp();
    std::shared_ptr<Number> ParseNumber();
    std::shared_ptr<LVal> ParseLVal();
    std::shared_ptr<ParenExp> ParseParenExp();
    std::shared_ptr<FuncFParam> ParseFuncFParam();
    std::shared_ptr<Cond> ParseCond();
    std::shared_ptr<AssignStmt> ParseAssignStmt();
    std::shared_ptr<Stmt> ParseStmt();
    std::shared_ptr<IfStmt> ParseIfStmt();
    std::shared_ptr<WhileStmt> ParseWhileStmt();
    std::shared_ptr<CtrlStmt> ParseCtrlStmt();
    std::shared_ptr<ReturnStmt> ParseReturnStmt();
    std::shared_ptr<Block> ParseBlock();
    std::shared_ptr<FuncDef> ParseFuncDef();
};
