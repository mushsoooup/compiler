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
  int print(int y, int x);
};

class UnaryExpr;
class MulExpr {
public:
  std::vector<UnaryExpr *> exprs;
  std::vector<OP> ops;
  int print(int y, int x);
};

class Callee;
class UnaryOp;
class LVal;
class Number;
class UnaryExpr {
public:
  std::variant<Callee *, UnaryOp *, AddExpr *, LVal *, Number *> expr;
  int print(int y, int x);
};

class UnaryOp {
public:
  OP op;
  UnaryExpr *expr = nullptr;
  int print(int y, int x);
};

class LVal {
public:
  std::string ident;
  std::vector<AddExpr *> exprs;
  int print(int y, int x);
};

class Number {
public:
  TYPE type;
  std::variant<int, float> val;
  int print(int y, int x);
};

class Callee {
public:
  std::string ident;
  std::vector<AddExpr *> args;
  int print(int y, int x);
};

class RelExpr {
public:
  std::vector<AddExpr *> exprs;
  std::vector<OP> ops;
  int print(int y, int x);
};

class EqExpr {
public:
  std::vector<RelExpr *> exprs;
  std::vector<OP> ops;
  int print(int y, int x);
};

class LAndExpr {
public:
  std::vector<EqExpr *> exprs;
  int print(int y, int x);
};

class LOrExpr {
public:
  std::vector<LAndExpr *> exprs;
  int print(int y, int x);
};

class Decl;
class FuncDef;
class CompUnit {
public:
  std::vector<std::variant<Decl *, FuncDef *>> defs;
  int print(int y, int x);
};

class VarDecl;
class Decl {
public:
  VarDecl *decl;
  int print(int y, int x);
};

class Array;
class InitVal {
public:
  std::variant<AddExpr *, Array *> val;
  int print(int y, int x);
};

class Array {
public:
  std::vector<InitVal *> vals;
  int print(int y, int x);
};

class VarDef;
class VarDecl {
public:
  TYPE type;
  bool is_const = false;
  std::vector<VarDef *> defs;
  int print(int y, int x);
};

class InitVal;
class VarDef {
public:
  bool is_const = false;
  std::string ident;
  std::vector<AddExpr *> exprs;
  InitVal *val = nullptr;
  int print(int y, int x);
};

class FuncFParam;
class Block;
class FuncDef {
public:
  TYPE type;
  std::string ident;
  std::vector<FuncFParam *> params;
  Block *block = nullptr;
  int print(int y, int x);
};

class FuncFParam {
public:
  TYPE type;
  std::string ident;
  bool array;
  std::vector<AddExpr *> exprs;
  int print(int y, int x);
};

class BlockItem;
class Block {
public:
  std::vector<BlockItem *> items;
  int print(int y, int x);
};

class Stmt;
class BlockItem {
public:
  std::variant<Decl *, Stmt *> item;
  int print(int y, int x);
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
  int print(int y, int x);
};

class AssignStmt {
public:
  LVal *lval = nullptr;
  AddExpr *expr = nullptr;
  int print(int y, int x);
};

class EvalStmt {
public:
  AddExpr *expr = nullptr;
  int print(int y, int x);
};

class IfStmt {
public:
  LOrExpr *cond = nullptr;
  Stmt *tStmt = nullptr, *fStmt = nullptr;
  int print(int y, int x);
};

class WhileStmt {
public:
  LOrExpr *cond = nullptr;
  Stmt *stmt = nullptr;
  int print(int y, int x);
};

class BreakStmt {
public:
  int print(int y, int x);
};
class ContinueStmt {
public:
  int print(int y, int x);
};
class ReturnStmt {
public:
  AddExpr *expr = nullptr;
  int print(int y, int x);
};
