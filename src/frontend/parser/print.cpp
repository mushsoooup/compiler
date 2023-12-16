//
// Created by zc175 on 2023/9/29.
//
#include "frontend/ast.h"
#include "frontend/parser.h"
#include <cstdio>
#include <cstring>
#include <variant>

// 1024 lines, 256 characters per line at max.
#define BUFFER_MAX_LINE (1024)
#define BUFFER_MAX_COLUMN (256)
char buffer[BUFFER_MAX_LINE][BUFFER_MAX_COLUMN] = {};

void put(int y, int x, const char *str) { strcpy(&buffer[y][x], str); }
void place(int y, int x, const char *str) {
  strncpy(&buffer[y][x], str, strlen(str));
}

int visit(auto &&arg, int y, int x) {
  return std::visit([&](auto &&arg) -> int { return arg->print(y, x); }, arg);
};

void fill(int y, int newY, int x) {
  for (int j = y + 1; j < newY; j++) {
    place(j, x, "|");
  }
}

const char *op2str(OP op) {
  switch (op) {
  case OP_ADD:
  case OP_POS:
    return " +";
  case OP_NEG:
  case OP_SUB:
    return " -";
  case OP_MUL:
    return " *";
  case OP_DIV:
    return " /";
  case OP_REM:
    return " %";
  case OP_NOT:
    return " !";
  case OP_LT:
    return " <";
  case OP_GT:
    return " >";
  case OP_LE:
    return " <=";
  case OP_GE:
    return " >=";
  case OP_EQ:
    return " ==";
  case OP_NE:
    return " !=";
  case OP_AND:
    return " &&";
  case OP_OR:
    return " ||";
  }
  return "unknown";
}

const char *type2str(TYPE type) {
  switch (type) {
  case TYPE_INT:
    return "int";
  case TYPE_VOID:
    return "void";
  case TYPE_FLOAT:
    return "float";
  }
  return "unknown";
}

void Parser::output_ast(std::ostream &out) const {
  memset(buffer, ' ', BUFFER_MAX_COLUMN * BUFFER_MAX_LINE);
  int y = comp_unit->print(0, 0);
  for (int i = 0; i < y; i++) {
    out << buffer[i] << "\n";
  }
};
// AddExpr
// +MulExpr
// +op
// \MulExpr
//
int AddExpr::print(int y, int x) {
  put(y++, x, "AddExpr");
  for (int i = 0; i < exprs.size(); i++) {
    int newY = exprs.at(i)->print(y, x + 2);
    if (i == exprs.size() - 1) {
      place(y, x, "\\-");
    } else {
      place(y, x, "+-");
      fill(y, newY, x);
      place(newY, x, "+-");
      put(newY++, x + 2, op2str(ops.at(i)));
    }
    y = newY;
  }
  return y;
}

// Similar ot AddExpr
int MulExpr::print(int y, int x) {
  put(y++, x, "MulExpr");
  for (int i = 0; i < exprs.size(); i++) {
    int newY = exprs.at(i)->print(y, x + 2);
    if (i == exprs.size() - 1) {
      place(y, x, "\\-");
    } else {
      place(y, x, "+-");
      fill(y, newY, x);
      place(newY, x, "+-");
      put(newY++, x + 2, op2str(ops.at(i)));
    }
    y = newY;
  }
  return y;
}

// UnaryExpr
// \Number
int UnaryExpr::print(int y, int x) {
  put(y++, x, "UnaryExpr");
  place(y, x, "\\-");
  return visit(expr, y, x + 2);
}

// UnaryOp
// +op
// \UnaryExpr
int UnaryOp::print(int y, int x) {
  put(y++, x, "UnaryOp");
  place(y, x, "+-");
  put(y++, x + 2, op2str(op));
  place(y, x, "\\-");
  return expr->print(y, x + 2);
}

// LVal
// +ident
// |
// +AddExpr
// |
// \AddExpr
int LVal::print(int y, int x) {
  put(y++, x, "LVal");
  if (exprs.size() == 0) {
    place(y, x, "\\-");
  } else {
    place(y, x, "+-");
  }
  put(y++, x + 2, ident.c_str());
  for (int i = 0; i < exprs.size(); i++) {
    int newY = exprs.at(i)->print(y, x + 2);
    if (i == exprs.size() - 1) {
      place(y, x, "\\-");
    } else {
      place(y, x, "+-");
      fill(y, newY, x);
    }
    y = newY;
  }
  return y;
}

// Number
// \0.03
int Number::print(int y, int x) {
  put(y++, x, "Number");
  place(y, x, "\\-");
  char buffer[256];
  if (val.index() == 0) {
    sprintf(buffer, "%d", std::get<int>(val));
    put(y, x + 2, buffer);
  } else {
    sprintf(buffer, "%f", std::get<float>(val));
    put(y, x + 2, buffer);
  }
  return y + 1;
}

// Similar to LVal
int Callee::print(int y, int x) {
  put(y++, x, "Callee");
  if (args.size() == 0) {
    place(y, x, "\\-");
  } else {
    place(y, x, "+-");
  }
  put(y++, x + 2, ident.c_str());
  for (int i = 0; i < args.size(); i++) {
    int newY = args.at(i)->print(y, x + 2);
    if (i == args.size() - 1) {
      place(y, x, "\\-");
    } else {
      place(y, x, "+-");
      fill(y, newY, x);
    }
    y = newY;
  }
  return y;
}

// Similar ot AddExpr
int RelExpr::print(int y, int x) {
  put(y++, x, "RelExpr");
  for (int i = 0; i < exprs.size(); i++) {
    int newY = exprs.at(i)->print(y, x + 2);
    if (i == exprs.size() - 1) {
      place(y, x, "\\-");
    } else {
      place(y, x, "+-");
      fill(y, newY, x);
      place(newY, x, "+-");
      put(newY++, x + 2, op2str(ops.at(i)));
    }
    y = newY;
  }
  return y;
}

// Similar ot AddExpr
int EqExpr::print(int y, int x) {
  put(y++, x, "EqExpr");
  for (int i = 0; i < exprs.size(); i++) {
    int newY = exprs.at(i)->print(y, x + 2);
    if (i == exprs.size() - 1) {
      place(y, x, "\\-");
    } else {
      place(y, x, "+-");
      fill(y, newY, x);
      place(newY, x, "+-");
      put(newY++, x + 2, op2str(ops.at(i)));
    }
    y = newY;
  }
  return y;
}

// LAndExpr
// +EqExpr
// |
// \EqExpr
int LAndExpr::print(int y, int x) {
  put(y++, x, "LAndExpr");
  for (int i = 0; i < exprs.size(); i++) {
    int newY = exprs.at(i)->print(y, x + 2);
    if (i == exprs.size() - 1) {
      place(y, x, "\\-");
    } else {
      place(y, x, "+-");
      fill(y, newY, x);
    }
    y = newY;
  }
  return y;
}

// Similar to LAndExpr
int LOrExpr::print(int y, int x) {
  put(y++, x, "LOrExpr");
  for (int i = 0; i < exprs.size(); i++) {
    int newY = exprs.at(i)->print(y, x + 2);
    if (i == exprs.size() - 1) {
      place(y, x, "\\-");
    } else {
      place(y, x, "+-");
      fill(y, newY, x);
    }
    y = newY;
  }
  return y;
}

// Similar to LAndExpr
int CompUnit::print(int y, int x) {
  put(y++, x, "CompUnit");
  for (int i = 0; i < defs.size(); i++) {
    int newY = visit(defs.at(i), y, x + 2);
    if (i == defs.size() - 1) {
      place(y, x, "\\-");
    } else {
      place(y, x, "+-");
      fill(y, newY, x);
    }
    y = newY;
  }
  return y;
}

// Similar to Number
int Decl::print(int y, int x) {
  put(y++, x, "Decl");
  place(y, x, "\\-");
  return decl->print(y, x + 2);
}

// Similar to Number
int InitVal::print(int y, int x) {
  put(y++, x, "InitVal");
  place(y, x, "\\-");
  return visit(val, y, x + 2);
}

// Similar to LAndExpr
int Array::print(int y, int x) {
  put(y++, x, "Array");
  for (int i = 0; i < vals.size(); i++) {
    int newY = vals.at(i)->print(y, x + 2);
    if (i == vals.size() - 1) {
      place(y, x, "\\-");
    } else {
      place(y, x, "+-");
      fill(y, newY, x);
    }
    y = newY;
  }
  return y;
}

// Similar to LVal
int VarDecl::print(int y, int x) {
  put(y++, x, "VarDecl");
  place(y, x, "+-");
  put(y++, x + 2, "Const");
  if (defs.size() == 0) {
    place(y, x, "\\-");
  } else {
    place(y, x, "+-");
  }
  put(y++, x + 2, type2str(type));
  for (int i = 0; i < defs.size(); i++) {
    int newY = defs.at(i)->print(y, x + 2);
    if (i == defs.size() - 1) {
      place(y, x, "\\-");
    } else {
      place(y, x, "+-");
      fill(y, newY, x);
    }
    y = newY;
  }
  return y;
}

// Similar to LVal
int VarDef::print(int y, int x) {
  put(y++, x, "VarDef");
  if (exprs.size() == 0) {
    place(y, x, "\\-");
  } else {
    place(y, x, "+-");
  }
  put(y++, x + 2, ident.c_str());
  for (int i = 0; i < exprs.size(); i++) {
    int newY = exprs.at(i)->print(y, x + 2);
    if (i == exprs.size() - 1 && !val) {
      place(y, x, "\\-");
    } else {
      place(y, x, "+-");
      fill(y, newY, x);
    }
    y = newY;
  }
  if (!val) {
    return y;
  }
  place(y, x, "\\-");
  return val->print(y, x + 2);
}

// Similar to LVal
int FuncDef::print(int y, int x) {
  put(y++, x, "FuncDef");
  place(y, x, "+-");
  put(y++, x + 2, type2str(type));
  if (params.size() == 0) {
    place(y, x, "\\-");
  } else {
    place(y, x, "+-");
  }
  put(y++, x + 2, ident.c_str());
  for (int i = 0; i < params.size(); i++) {
    int newY = params.at(i)->print(y, x + 2);
    if (i == params.size() - 1 && !block) {
      place(y, x, "\\-");
    } else {
      place(y, x, "+-");
      fill(y, newY, x);
    }
    y = newY;
  }
  if (!block) {
    return y;
  }
  place(y, x, "\\-");
  return block->print(y, x + 2);
}

// Similar to LVal
int FuncFParam::print(int y, int x) {
  put(y++, x, "FuncFParam");
  place(y, x, "+-");
  put(y++, x + 2, type2str(type));
  place(y, x, "+-");
  put(y++, x + 2, ident.c_str());
  if (exprs.size() == 0) {
    place(y, x, "\\-");
  } else {
    place(y, x, "+-");
  }
  if (array) {
    put(y++, x + 2, "array");
  } else {
    put(y++, x + 2, "normal");
  }
  for (int i = 0; i < exprs.size(); i++) {
    int newY = exprs.at(i)->print(y, x + 2);
    if (i == exprs.size() - 1) {
      place(y, x, "\\-");
    } else {
      place(y, x, "+-");
      fill(y, newY, x);
    }
    y = newY;
  }
  return y;
}

// Similar to LAndExpr
int Block::print(int y, int x) {
  put(y++, x, "Block");
  for (int i = 0; i < items.size(); i++) {
    int newY = items.at(i)->print(y, x + 2);
    if (i == items.size() - 1) {
      place(y, x, "\\-");
    } else {
      place(y, x, "+-");
      fill(y, newY, x);
    }
    y = newY;
  }
  return y;
}

// Similar to Number
int BlockItem::print(int y, int x) {
  put(y++, x, "BlockItem");
  place(y, x, "\\-");
  return visit(item, y, x + 2);
}

// Similar to Number
int Stmt::print(int y, int x) {
  put(y++, x, "Stmt");
  place(y, x, "\\-");
  return visit(stmt, y, x + 2);
}

int AssignStmt::print(int y, int x) {
  put(y++, x, "AssignStmt");
  place(y, x, "+-");
  int newY = lval->print(y, x + 2);
  fill(y, newY, x);
  y = newY;
  place(y, x, "\\-");
  return expr->print(y, x + 2);
}

int EvalStmt::print(int y, int x) {
  put(y++, x, "EvalStmt");
  place(y, x, "\\-");
  return expr->print(y, x + 2);
}

int IfStmt::print(int y, int x) {
  put(y++, x, "IfStmt");
  place(y, x, "+-");
  int newY = cond->print(y, x + 2);
  fill(y, newY, x);
  y = newY;
  if (!fStmt) {
    place(y, x, "\\-");
    return tStmt->print(y, x + 2);
  } else {
    place(y, x, "+-");
    newY = tStmt->print(y, x + 2);
  }
  fill(y, newY, x);
  y = newY;
  place(y, x, "\\-");
  return tStmt->print(y, x + 2);
}

int WhileStmt::print(int y, int x) {
  put(y++, x, "WhileStmt");
  place(y, x, "+-");
  int newY = cond->print(y, x + 2);
  fill(y, newY, x);
  y = newY;
  place(y, x, "\\-");
  return stmt->print(y, x + 2);
}

int BreakStmt::print(int y, int x) {
  put(y++, x, "BreakStmt");
  return y;
}

int ContinueStmt::print(int y, int x) {
  put(y++, x, "ContinueStmt");
  return y;
}

int ReturnStmt::print(int y, int x) {
  put(y++, x, "ReturnStmt");
  place(y, x, "\\-");
  return expr->print(y, x + 2);
}
