//
// Created by zc175 on 2023/9/29.
//
#pragma once
#include "frontend/ast.h"
#include "frontend/lexer.h"
#include <string>
#include <utility>

class Parser {

public:
  CompUnit *comp_unit;
  explicit Parser(TokenList _token_list) : token_list(_token_list){};
  void parse();
  void output_ast(std::ostream &) const;

private:
  TokenList token_list;
  Token &read();
  Token &peek(size_t pos = 0);
  AddExpr *ParseAddExpr();
  MulExpr *ParseMulExpr();
  UnaryExpr *ParseUnaryExpr();
  UnaryOp *ParseUnaryOp();
  LVal *ParseLVal();
  Number *ParseNumber();
  Callee *ParseCallee();
  RelExpr *ParseRelExpr();
  EqExpr *ParseEqExpr();
  LAndExpr *ParseLAndExpr();
  LOrExpr *ParseLOrExpr();
  CompUnit *ParseCompUnit();
  Decl *ParseDecl();
  ConstDecl *ParseConstDecl();
  ConstDef *ParseConstDef();
  InitVal *ParseInitVal();
  Array *ParseArray();
  VarDecl *ParseVarDecl();
  VarDef *ParseVarDef();
  FuncDef *ParseFuncDef();
  FuncFParam *ParseFuncFParam();
  Block *ParseBlock();
  BlockItem *ParseBlockItem();
  Stmt *ParseStmt();
  AssignStmt *ParseAssignStmt();
  EvalStmt *ParseEvalStmt();
  IfStmt *ParseIfStmt();
  WhileStmt *ParseWhileStmt();
  BreakStmt *ParseBreakStmt();
  ContinueStmt *ParseContinueStmt();
  ReturnStmt *ParseReturnStmt();
};
