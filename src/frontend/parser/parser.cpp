//
// Created by zc175 on 2023/9/29.
//

#include "frontend/parser.h"
#include "frontend/ast.h"
#include "frontend/lexer.h"
#include "utils/conv.h"

Token &Parser::read() { return token_list.read(); }

Token &Parser::peek(size_t pos) { return token_list.peek(pos); }

void Parser::parse() { comp_unit = nullptr; }

// AddExpr -> MulExpr { ('+' | '-') MulExpr }
AddExpr *Parser::ParseAddExpr() {
  auto res = new AddExpr;
  while (true) {
    res->exprs.push_back(ParseMulExpr());
    if (!peek().expect({TK_OP_ADD, TK_OP_SUB})) {
      break;
    }
    auto tk = read();
    if (tk.expect({TK_OP_ADD})) {
      res->ops.push_back(OP_ADD);
    } else {
      tk.must({TK_OP_SUB});
      res->ops.push_back(OP_SUB);
    }
  }
  return res;
};

// MulExpr -> UnaryExpr { ('*' | '/' | '%') UnaryExpr }
MulExpr *Parser::ParseMulExpr() {
  auto res = new MulExpr;
  while (true) {
    res->exprs.push_back(ParseUnaryExpr());
    if (!peek().expect({TK_OP_MUL, TK_OP_DIV, TK_OP_MOD})) {
      break;
    }
    auto tk = read();
    if (tk.expect({TK_OP_MUL})) {
      res->ops.push_back(OP_MUL);
    } else if (tk.expect({TK_OP_DIV})) {
      res->ops.push_back(OP_DIV);
    } else {
      tk.must({TK_OP_MOD});
      res->ops.push_back(OP_REM);
    }
  }
  return res;
};

// UnaryExpr → UnaryOp | '(' AddExpr ')' | Number | Callee | LVal
UnaryExpr *Parser::ParseUnaryExpr() {
  auto res = new UnaryExpr;
  auto tk = peek();
  if (tk.expect({TK_OP_ADD, TK_OP_SUB, TK_OP_NOT})) {
    res->expr = ParseUnaryOp();
  } else if (tk.expect({TK_LPAREN})) {
    read();
    res->expr = ParseAddExpr();
    read().must({TK_RPAREN});
  } else if (tk.expect({TK_CONST_INT, TK_CONST_FLOAT})) {
    res->expr = ParseNumber();
  } else {
    if (peek(1).expect({TK_LPAREN})) {
      res->expr = ParseCallee();
    } else {
      res->expr = ParseLVal();
    }
  }
  return res;
};

// UnaryOp -> ('+' | '−' | '!') UnaryExpr
UnaryOp *Parser::ParseUnaryOp() {
  auto res = new UnaryOp;
  auto tk = read();
  if (tk.expect({TK_OP_ADD})) {
    res->op = OP_POS;
  } else if (tk.expect({TK_OP_SUB})) {
    res->op = OP_SUB;
  } else {
    tk.must({TK_OP_NOT});
    res->op = OP_NEG;
  }
  res->expr = ParseUnaryExpr();
  return res;
};

// LVal → IDENT {'[' AddExpr ']'}
LVal *Parser::ParseLVal() {
  auto res = new LVal;
  auto tk = read();
  tk.must({TK_IDENT});
  res->ident = tk.raw;
  while (peek().expect({TK_LBRACE})) {
    read();
    res->exprs.push_back(ParseAddExpr());
    read().must({TK_RBRACE});
  }
  return res;
};

// Number → INT | FLOAT
Number *Parser::ParseNumber() {
  auto res = new Number;
  auto tk = read();
  if (tk.expect({TK_CONST_INT})) {
    res->type = TYPE_INT;
    res->val = ttoi(tk);
  } else {
    tk.must({TK_CONST_FLOAT});
    res->type = TYPE_FLOAT;
    res->val = ttof(tk);
  }
  return res;
};

// Callee -> IDENT '(' AddExpr { ',' AddExpr } ')'
Callee *Parser::ParseCallee() {
  auto res = new Callee;
  auto tk = read();
  tk.must({TK_IDENT});
  res->ident = tk.raw;
  read().must({TK_LBRACE});
  while (true) {
    res->args.push_back(ParseAddExpr());
    if (!peek().expect({TK_COMMA})) {
      break;
    }
    read();
  }
  read().must({TK_RBRACE});
  return res;
};

// RelExpr -> AddExpr { ('<' | '>' | '<=' | '>=') AddExpr }
RelExpr *Parser::ParseRelExpr() {
  auto res = new RelExpr;
  res->exprs.push_back(ParseAddExpr());
  while (peek().expect({TK_OP_LT, TK_OP_GT, TK_OP_LE, TK_OP_GE})) {
    auto tk = read();
    if (tk.expect({TK_OP_LT})) {
      res->ops.push_back(OP_LT);
    } else if (tk.expect({TK_OP_GT})) {
      res->ops.push_back(OP_GT);
    } else if (tk.expect({TK_OP_LE})) {
      res->ops.push_back(OP_LE);
    } else {
      res->ops.push_back(OP_GE);
    }
    res->exprs.push_back(ParseAddExpr());
  }
  return res;
};

// EqExpr → RelExpr { ('==' | '!=') RelExpr }
EqExpr *Parser::ParseEqExpr() {
  auto res = new EqExpr;
  res->exprs.push_back(ParseRelExpr());
  while (peek().expect({TK_OP_EQ, TK_OP_NE})) {
    auto tk = read();
    if (tk.expect({TK_OP_EQ})) {
      res->ops.push_back(OP_EQ);
    } else {
      res->ops.push_back(OP_NE);
    }
    res->exprs.push_back(ParseRelExpr());
  }
  return res;
};

// LAndExpr → EqExpr { '&&' EqExpr }
LAndExpr *Parser::ParseLAndExpr() {
  auto res = new LAndExpr;
  res->exprs.push_back(ParseEqExpr());
  while (peek().expect({TK_OP_AND})) {
    auto tk = read();
    res->exprs.push_back(ParseEqExpr());
  }
  return res;
};

// LOrExpr → LAndExpr { '||' LAndExpr }
LOrExpr *Parser::ParseLOrExpr() {
  auto res = new LOrExpr;
  res->exprs.push_back(ParseLAndExpr());
  while (peek().expect({TK_OP_OR})) {
    auto tk = read();
    res->exprs.push_back(ParseLAndExpr());
  }
  return res;
};

// CompUnit -> { (Decl | Funcdef) }
CompUnit *Parser::ParseCompUnit() {
  auto res = new CompUnit;
  while (!token_list.empty()) {
    if (peek(2).expect({TK_LBRACE})) {
      res->defs.push_back(ParseFuncDef());
    } else {
      res->defs.push_back(ParseDecl());
    }
  }
  return res;
};

// Decl -> ConstDecl | VarDecl
Decl *Parser::ParseDecl() {
  auto res = new Decl;
  if (peek().expect({TK_KEYWORD_CONST})) {
    res->decl = ParseConstDecl();
  } else {
    res->decl = ParseVarDecl();
  }
  return res;
};

// ConstDecl -> 'const' Type ConstDef { ',' ConstDef } ';'
ConstDecl *Parser::ParseConstDecl() {
  auto res = new ConstDecl;
  read().must({TK_KEYWORD_CONST});
  auto tk = read();
  if (tk.expect({TK_KEYWORD_INT})) {
    res->type = TYPE_INT;
  } else {
    read().must({TK_KEYWORD_FLOAT});
    res->type = TYPE_FLOAT;
  }
  while (true) {
    res->defs.push_back(ParseConstDef());
    if (!peek().expect({TK_COMMA})) {
      break;
    }
    read();
  }
  read().must({TK_SEMI});
  return res;
};

// ConstDef -> IDENT { '[' AddExpr ']' } '=' InitVal
ConstDef *Parser::ParseConstDef() {
  auto res = new ConstDef;
  auto tk = read();
  tk.must({TK_IDENT});
  res->ident = tk.raw;
  while (peek().expect({TK_LBRACE})) {
    read();
    res->exprs.push_back(ParseAddExpr());
    read().must({TK_RBRACE});
  }
  read().must({TK_ASSIGN});
  res->val = ParseInitVal();
  return res;
};

// InitVal → AddExpr | Array
InitVal *Parser::ParseInitVal() {
  auto res = new InitVal;
  if (peek().expect({TK_LBRACK})) {
    res->val = ParseArray();
  } else {
    res->val = ParseAddExpr();
  }
  return res;
};

// Array -> '{' [ InitVal { ',' InitVal } ] '}'
Array *Parser::ParseArray() {
  auto res = new Array;
  read().must({TK_LBRACK});
  if (peek().expect({TK_RBRACK})) {
    read();
    return res;
  }
  while (true) {
    res->vals.push_back(ParseInitVal());
    if (!peek().expect({TK_COMMA})) {
      break;
    }
    read();
  }
  return res;
};

// VarDecl → Type VarDef { ',' VarDef } ';'
VarDecl *Parser::ParseVarDecl() {
  auto res = new VarDecl;
  auto tk = read();
  if (tk.expect({TK_KEYWORD_INT})) {
    res->type = TYPE_INT;
  } else {
    tk.must({TK_KEYWORD_FLOAT});
    res->type = TYPE_FLOAT;
  }
  while (true) {
    res->defs.push_back(ParseVarDef());
    if (!peek().expect({TK_COMMA})) {
      break;
    }
    read();
  }
  read().must({TK_SEMI});
  return res;
};

// VarDef → IDENT { '[' AddExpr ']' } [ '=' InitVal ]
VarDef *Parser::ParseVarDef() {
  auto res = new VarDef;
  auto tk = read();
  tk.must({TK_IDENT});
  res->ident = tk.raw;
  while (peek().expect({TK_LBRACE})) {
    read();
    res->exprs.push_back(ParseAddExpr());
    read().must({TK_RBRACE});
  }
  if (peek().expect({TK_ASSIGN})) {
    read();
    res->val = ParseInitVal();
  }
  return res;
};

// FuncDef → Type IDENT '(' [FuncFParam] { ',' FuncFParam } ')' Block
FuncDef *Parser::ParseFuncDef() {
  auto res = new FuncDef;
  auto tk = read();
  if (tk.expect({TK_KEYWORD_VOID})) {
    res->type = TYPE_VOID;
  } else if (tk.expect({TK_KEYWORD_INT})) {
    res->type = TYPE_INT;
  } else {
    tk.must({TK_KEYWORD_FLOAT});
    res->type = TYPE_FLOAT;
  }
  tk = read();
  tk.must({TK_IDENT});
  res->ident = tk.raw;
  read().must({TK_LBRACE});
  if (!peek().expect({TK_RBRACE})) {
    while (true) {
      res->params.push_back(ParseFuncFParam());
      if (!peek().expect({TK_COMMA})) {
        break;
      }
      read();
    }
  }
  read();
  res->block = ParseBlock();
  return res;
};

// FuncFParam → Type IDENT [ '[' ']' { '[' AddExpr ']' } ]
FuncFParam *Parser::ParseFuncFParam() {
  auto res = new FuncFParam;
  auto tk = read();
  if (tk.expect({TK_KEYWORD_INT})) {
    res->type = TYPE_INT;
  } else {
    tk.must({TK_KEYWORD_FLOAT});
    res->type = TYPE_FLOAT;
  }
  tk = read();
  tk.must({TK_IDENT});
  res->ident = tk.raw;
  if (!peek().expect({TK_LBRACE})) {
    return res;
  }
  res->array = true;
  read().must({TK_LBRACE});
  read().must({TK_RBRACE});
  while (peek().expect({TK_LBRACE})) {
    read();
    res->exprs.push_back(ParseAddExpr());
    read().must({TK_RBRACE});
  }
  return res;
};

// Block → '{' { BlockItem } '}'
Block *Parser::ParseBlock() {
  auto res = new Block;
  read().must({TK_LBRACK});
  while (!peek().expect({TK_RBRACK})) {
    res->items.push_back(ParseBlockItem());
  }
  read().must({TK_RBRACK});
  return res;
};

// BlockItem → Decl | Stmt
BlockItem *Parser::ParseBlockItem() {
  auto res = new BlockItem;
  if (peek().is_type()) {
    res->item = ParseDecl();
  } else {
    res->item = ParseStmt();
  }
  return res;
};

// Stmt → Block | IfStmt | WhileStmt | BreakStmt |
// ContinueStmt | ReturnStmt | AssignStmt | EvalStmt
Stmt *Parser::ParseStmt() {
  auto res = new Stmt;
  if (peek().expect({TK_LBRACK})) {
    res->stmt = ParseBlock();
  } else if (peek().expect({TK_KEYWORD_IF})) {
    res->stmt = ParseIfStmt();
  } else if (peek().expect({TK_KEYWORD_WHILE})) {
    res->stmt = ParseWhileStmt();
  } else if (peek().expect({TK_KEYWORD_BREAK})) {
    res->stmt = ParseBreakStmt();
  } else if (peek().expect({TK_KEYWORD_CONTINUE})) {
    res->stmt = ParseContinueStmt();
  } else if (peek().expect({TK_KEYWORD_RETURN})) {
    res->stmt = ParseReturnStmt();
  } else {
    // search for ; or =, true for ;, false for =
    bool flag = false;
    for (int i = 0;; i++) {
      auto tk = peek(i);
      if (tk.expect({TK_SEMI})) {
        flag = true;
        break;
      } else if (tk.expect({TK_ASSIGN})) {
        flag = false;
        break;
      }
    }
    if (flag) {
      res->stmt = ParseEvalStmt();
    } else {
      res->stmt = ParseAssignStmt();
    }
  }
  return res;
};

// AssignStmt -> LVal '=' AddExpr ';'
AssignStmt *Parser::ParseAssignStmt() {
  auto res = new AssignStmt;
  res->lval = ParseLVal();
  read().must({TK_ASSIGN});
  res->expr = ParseAddExpr();
  read().must({TK_SEMI});
  return res;
};

// EvalStmt -> [AddExpr] ';'
EvalStmt *Parser::ParseEvalStmt() {
  auto res = new EvalStmt;
  if (peek().expect({TK_SEMI})) {
    read();
    return res;
  }
  res->expr = ParseAddExpr();
  return res;
};

// IfStmt -> 'if' '( LOrExpr ')' Stmt [ 'else' Stmt ]
IfStmt *Parser::ParseIfStmt() {
  auto res = new IfStmt;
  read().must({TK_KEYWORD_IF});
  read().must({TK_LBRACE});
  res->cond = ParseLOrExpr();
  read().must({TK_RBRACE});
  res->tStmt = ParseStmt();
  if (!peek().expect({TK_KEYWORD_ELSE})) {
    return res;
  }
  read();
  res->fStmt = ParseStmt();
  return res;
};

// WhileStmt -> 'while' '(' LOrExpr ')' Stmt
WhileStmt *Parser::ParseWhileStmt() {
  auto res = new WhileStmt;
  read().must({TK_KEYWORD_WHILE});
  read().must({TK_LBRACE});
  res->cond = ParseLOrExpr();
  read().must({TK_RBRACE});
  res->stmt = ParseStmt();
  return res;
};

// BreakStmt -> 'break' ';'
BreakStmt *Parser::ParseBreakStmt() {
  auto res = new BreakStmt;
  read().must({TK_KEYWORD_BREAK});
  read().must({TK_SEMI});
  return res;
};

// ContinueStmt -> 'continue' ';'
ContinueStmt *Parser::ParseContinueStmt() {
  auto res = new ContinueStmt;
  read().must({TK_KEYWORD_CONTINUE});
  read().must({TK_SEMI});
  return res;
};

// ReturnStmt -> 'return' [ AddExpr ] ';'
ReturnStmt *Parser::ParseReturnStmt() {
  auto res = new ReturnStmt;
  read().must({TK_KEYWORD_RETURN});
  if (peek().expect({TK_SEMI})) {
    read();
    return res;
  }
  res->expr = ParseAddExpr();
  read().must({TK_SEMI});
  return res;
};
