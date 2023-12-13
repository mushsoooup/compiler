//
// Created by zc175 on 2023/9/29.
//
#pragma once
#include "frontend/lexer.h"
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

enum TYPE {
  TYPE_VOID,
  TYPE_INT,
  TYPE_FLOAT,
};

enum OP {
  OP_ADD,
  OP_SUB,
  OP_MUL,
  OP_DIV,
  OP_REM,
  OP_POS,
  OP_NEG,
  OP_NOT,
  OP_LT,
  OP_GT,
  OP_LE,
  OP_GE,
  OP_EQ,
  OP_NE,
  OP_AND,
  OP_OR,
};

class MulExpr;
class AddExpr {
public:
  std::vector<MulExpr *> exprs;
  std::vector<OP> ops;
};

class UnaryExpr;
class MulExpr {
public:
  std::vector<UnaryExpr *> exprs;
  std::vector<OP> ops;
};

class Callee;
class UnaryOp;
class LVal;
class Number;
class UnaryExpr {
public:
  std::variant<Callee *, UnaryOp *, AddExpr *, LVal *, Number *> expr;
};

class UnaryOp {
public:
  OP op;
  UnaryExpr *expr = nullptr;
};

class LVal {
public:
  std::string ident;
  std::vector<AddExpr *> exprs;
};

class Number {
public:
  TYPE type;
  std::variant<int, float> val;
};

class Callee {
public:
  std::string ident;
  std::vector<AddExpr *> args;
};

class RelExpr {
public:
  std::vector<AddExpr *> exprs;
  std::vector<OP> ops;
};

class EqExpr {
public:
  std::vector<RelExpr *> exprs;
  std::vector<OP> ops;
};

class LAndExpr {
public:
  std::vector<EqExpr *> exprs;
};

class LOrExpr {
public:
  std::vector<LAndExpr *> exprs;
};

class Decl;
class FuncDef;
class CompUnit {
public:
  std::vector<std::variant<Decl *, FuncDef *>> defs;
};

class ConstDecl;
class VarDecl;
class Decl {
public:
  std::variant<ConstDecl *, VarDecl *> decl;
};

class ConstDef;
class ConstDecl {
public:
  TYPE type;
  std::vector<ConstDef *> defs;
};

class InitVal;
class ConstDef {
public:
  std::string ident;
  std::vector<AddExpr *> exprs;
  InitVal *val = nullptr;
};

class Array;
class InitVal {
public:
  std::variant<AddExpr *, Array *> val;
};

class Array {
public:
  std::vector<InitVal *> vals;
};

class VarDef;
class VarDecl {
public:
  TYPE type;
  std::vector<VarDef *> defs;
};

class InitVal;
class VarDef {
public:
  std::string ident;
  std::vector<AddExpr *> exprs;
  InitVal *val = nullptr;
};

class FuncFParam;
class Block;
class FuncDef {
public:
  TYPE type;
  std::string ident;
  std::vector<FuncFParam *> params;
  Block *block = nullptr;
};

class FuncFParam {
public:
  TYPE type;
  std::string ident;
  bool array;
  std::vector<AddExpr *> exprs;
};

class BlockItem;
class Block {
public:
  std::vector<BlockItem *> items;
};

class Stmt;
class BlockItem {
public:
  std::variant<Decl *, Stmt *> item;
};

class AssignStmt;
class EvalStmt;
class IfStmt;
class WhileStmt;
class BreakStmt;
class ContinueStmt;
class ReturnStmt;
class Stmt {
public:
  std::variant<AssignStmt *, EvalStmt *, IfStmt *, Block *, WhileStmt *,
               BreakStmt *, ContinueStmt *, ReturnStmt *>
      stmt;
};

class AssignStmt {
public:
  LVal *lval = nullptr;
  AddExpr *expr = nullptr;
};

class EvalStmt {
public:
  AddExpr *expr = nullptr;
};

class IfStmt {
public:
  LOrExpr *cond = nullptr;
  Stmt *tStmt = nullptr, *fStmt = nullptr;
};

class WhileStmt {
public:
  LOrExpr *cond = nullptr;
  Stmt *stmt = nullptr;
};

class BreakStmt {};
class ContinueStmt {};
class ReturnStmt {
public:
  AddExpr *expr = nullptr;
};
