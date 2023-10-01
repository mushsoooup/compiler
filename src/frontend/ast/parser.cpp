//
// Created by zc175 on 2023/9/29.
//
#include <memory>

#include "frontend/parser.h"
#include <iostream>

Token &Parser::read() {
    return token_list.read();
}

Token &Parser::peek(size_t pos) {
    return token_list.peek(pos);
}

void Parser::parse() {
    comp_unit = ParseCompUnit();
}

// CompUnit -> (Decl)*
std::shared_ptr<CompUnit> Parser::ParseCompUnit() {
    auto result = std::make_shared<CompUnit>();
    while (!token_list.empty()) {
        if (peek(2).expect({TK_LPAREN})) {
            result->decls.emplace_back(ParseFuncDef());
        } else {
            result->decls.emplace_back(ParseVarDecl());
        }
    }
    return result;
}

// VarDecl -> BType VarDef (, VarDef)* ;
std::shared_ptr<VarDecl> Parser::ParseVarDecl() {
    auto &token = read();
    bool is_const = false;
    if (token.expect({TK_KEYWORD_CONST})) {
        is_const = true;
        token = read();
    }
    token.must({TK_KEYWORD_INT, TK_KEYWORD_FLOAT});
    auto result = std::make_shared<VarDecl>(token, is_const);
    result->defs.emplace_back(ParseVarDef());
    while (peek().expect({TK_COMMA})) {
        read();
        result->defs.emplace_back(ParseVarDef());
    }
    read().must({TK_SEMI});
    return result;
}

// VarDef -> ident (= Exp)?
std::shared_ptr<VarDef> Parser::ParseVarDef() {
    auto &token = read();
    token.must({TK_IDENT});
    auto result = std::make_shared<VarDef>(token);
    // array
    while (peek().expect({TK_LPAREN})) {
        read();
        result->dims.emplace_back(ParseExp());
        read().must({TK_RPAREN});
    }
    read().must({TK_ASSIGN});
    if (peek().expect({TK_COMMA, TK_SEMI})) {
        // Uninitialized variable
        result->exp = nullptr;
    } else if (peek().expect({TK_LBRACK})){
        // Array

    } else {
        // Initialized variable
        result->exp = ParseExp();
    }
    return result;
}

// Exp -> AddExp
std::shared_ptr<Exp> Parser::ParseExp() {
    return std::make_shared<Exp>(ParseAddExp());
}

// MulExp -> UnaryExp (MulOp UnaryExp)*
// MulOp -> * | / | %
std::shared_ptr<MulExp> Parser::ParseMulExp() {
    auto result = std::make_shared<MulExp>();
    result->exps.emplace_back(ParseUnaryExp());
    while (peek().expect({TK_OP_MUL, TK_OP_DIV, TK_OP_MOD})) {
        result->ops.emplace_back(&read());
        result->exps.emplace_back(ParseUnaryExp());
    }
    return result;
}

// UnaryExp -> PrimaryExp | Callee | UnaryOp
std::shared_ptr<UnaryExp> Parser::ParseUnaryExp() {
    auto &token = peek();
    auto result = std::make_shared<UnaryExp>();
    if (token.expect({TK_OP_ADD, TK_OP_SUB, TK_OP_NOT})) {
        result->exp = ParseUnaryOp();
    } else if (token.expect({TK_IDENT}) && peek(1).expect({TK_LPAREN})) {
        result->exp = ParseCallee();
    } else {
        result->exp = ParsePrimaryExp();
    }
    return result;
}

// PrimaryExp -> ParenExp | LVal | Number
std::shared_ptr<PrimaryExp> Parser::ParsePrimaryExp() {
    auto result = std::make_shared<PrimaryExp>();
    if (peek().expect({TK_CONST_INT, TK_CONST_FLOAT})) {
        result->exp = ParseNumber();
    } else if (peek().expect({TK_IDENT})) {
        result->exp = ParseLVal();
    } else {
        peek().must({TK_LPAREN});
        result->exp = ParseParenExp();
    }
    return result;
}

// Callee -> ident()
std::shared_ptr<Callee> Parser::ParseCallee() {
    auto &token = read();
    token.must({TK_IDENT});
    auto result = std::make_shared<Callee>(token);
    read().must({TK_LPAREN});
    // TODO: Parse FuncRParams
    if (peek().expect({TK_RPAREN})) {
        read();
        return result;
    } else {
        result->params.emplace_back(ParseExp());
        while (peek().expect({TK_COMMA})) {
            read();
            result->params.emplace_back(ParseExp());
        }
    }
    read().must({TK_RPAREN});
    return result;
}

// UnaryOp -> UnaryOperator UnaryExp
// UnaryOperator -> + | - | !
std::shared_ptr<UnaryOp> Parser::ParseUnaryOp() {
    auto &token = read();
    token.must({TK_OP_ADD, TK_OP_SUB, TK_OP_NOT});
    auto result = std::make_shared<UnaryOp>(token);
    result->exp = ParseUnaryExp();
    return result;
}

// Number -> int | float
std::shared_ptr<Number> Parser::ParseNumber() {
    auto &token = read();
    token.must({TK_CONST_INT, TK_CONST_FLOAT});
    return std::make_shared<Number>(token);
}

// LVal -> ident
std::shared_ptr<LVal> Parser::ParseLVal() {
    auto &token = read();
    token.must({TK_IDENT});
    return std::make_shared<LVal>(token);
}

// ParenExp -> (Exp)
std::shared_ptr<ParenExp> Parser::ParseParenExp() {
    auto &token = read();
    token.must({TK_LPAREN});
    auto result = std::make_shared<ParenExp>(ParseExp());
    read().must({TK_RPAREN});
    return result;
}

// AddExp -> MulExp (AddOp MulExp)*
// AddOp -> + | -
std::shared_ptr<AddExp> Parser::ParseAddExp() {
    auto result = std::make_shared<AddExp>();
    result->exps.emplace_back(ParseMulExp());
    while (peek().expect({TK_OP_ADD, TK_OP_SUB})) {
        result->ops.emplace_back(&read());
        result->exps.emplace_back(ParseMulExp());
    }
    return result;
}

// RelExp -> AddExp (RelOp AddExp)*
// RelOp -> < | > | <= | >=
std::shared_ptr<RelExp> Parser::ParseRelExp() {
    auto result = std::make_shared<RelExp>();
    result->exps.emplace_back(ParseAddExp());
    while (peek().expect({TK_OP_LT, TK_OP_GT, TK_OP_LE, TK_OP_GE})) {
        result->ops.emplace_back(&read());
        result->exps.emplace_back(ParseAddExp());
    }
    return result;
}

// EqExp -> RelExp (EqOp RelExp)*
// EqOp -> == | !=
std::shared_ptr<EqExp> Parser::ParseEqExp() {
    auto result = std::make_shared<EqExp>();
    result->exps.emplace_back(ParseRelExp());
    while (peek().expect({TK_OP_EQ, TK_OP_NE})) {
        result->ops.emplace_back(&read());
        result->exps.emplace_back(ParseRelExp());
    }
    return result;
}

// LAndExp -> EqExp (&& EqExp)*
std::shared_ptr<LAndExp> Parser::ParseLAndExp() {
    auto result = std::make_shared<LAndExp>();
    result->exps.emplace_back(ParseEqExp());
    while (peek().expect({TK_OP_AND})) {
        read();
        result->exps.emplace_back(ParseEqExp());
    }
    return result;
}

// LOrExp -> LAndExp (|| LAndExp)*
std::shared_ptr<LOrExp> Parser::ParseLOrExp() {
    auto result = std::make_shared<LOrExp>();
    result->exps.emplace_back(ParseLAndExp());
    while (peek().expect({TK_OP_OR})) {
        read();
        result->exps.emplace_back(ParseLAndExp());
    }
    return result;
}

// FuncFParam -> btype ident
std::shared_ptr<FuncFParam> Parser::ParseFuncFParam() {
    auto &btype = read();
    btype.must({TK_KEYWORD_VOID, TK_KEYWORD_INT, TK_KEYWORD_FLOAT});
    auto &ident = read();
    ident.must({TK_IDENT});
    return std::make_shared<FuncFParam>(btype, ident);
}

// Cond -> LOrExp
std::shared_ptr<Cond> Parser::ParseCond() {
    auto result = std::make_shared<Cond>();
    result->exp = ParseLOrExp();
    return result;
}

// AssignStmt -> LVal = Exp;
std::shared_ptr<AssignStmt> Parser::ParseAssignStmt() {
    auto result = std::make_shared<AssignStmt>();
    result->lval = ParseLVal();
    read().must({TK_ASSIGN});
    result->exp = ParseExp();
    read().must({TK_SEMI});
    return result;
}

// IfStmt -> if (Cond) Stmt [else Stmt]
std::shared_ptr<IfStmt> Parser::ParseIfStmt() {
    auto result = std::make_shared<IfStmt>();
    read().must({TK_KEYWORD_IF});
    read().must({TK_LPAREN});
    result->cond = ParseCond();
    read().must({TK_RPAREN});
    result->body = ParseStmt();
    result->else_body = nullptr;
    if (peek().expect({TK_KEYWORD_ELSE})) {
        read();
        result->else_body = ParseStmt();
    }
    return result;
}

// WhileStmt -> while (Cond) Stmt
std::shared_ptr<WhileStmt> Parser::ParseWhileStmt() {
    auto result = std::make_shared<WhileStmt>();
    read().must({TK_KEYWORD_WHILE});
    read().must({TK_LPAREN});
    result->cond = ParseCond();
    read().must({TK_RPAREN});
    result->body = ParseStmt();
    return result;
}

// CtrlStmt -> break; | continue;
std::shared_ptr<CtrlStmt> Parser::ParseCtrlStmt() {
    auto result = std::make_shared<CtrlStmt>();
    if (peek().expect({TK_KEYWORD_BREAK})) {
        read();
        result->type = true;
    } else {
        read().must({TK_KEYWORD_CONTINUE});
        result->type = false;
    }
    read().must({TK_SEMI});
    return result;
}

// ReturnStmt -> return [Exp];
std::shared_ptr<ReturnStmt> Parser::ParseReturnStmt() {
    read().must({TK_KEYWORD_RETURN});
    auto result = std::make_shared<ReturnStmt>();
    if (peek().expect({TK_SEMI})) {
        read();
        return result;
    }
    result->exp = ParseExp();
    read().must({TK_SEMI});
    return result;
}

// Block -> { {Decl | Stmt} }
std::shared_ptr<Block> Parser::ParseBlock() {
    read().must({TK_LBRACK});
    auto result = std::make_shared<Block>();
    while (!peek().expect({TK_RBRACK})) {
        if (peek().expect({TK_KEYWORD_CONST, TK_KEYWORD_INT, TK_KEYWORD_FLOAT})) {
            result->items.emplace_back(ParseVarDecl());
        } else {
            result->items.emplace_back(ParseStmt());
        }
    }
    read();
    return result;
}

// FuncDef -> FuncType ident ([FuncFParams]) Block
// FuncType -> void | int | float
std::shared_ptr<FuncDef> Parser::ParseFuncDef() {
    auto &functype= read();
    functype.must({TK_KEYWORD_VOID, TK_KEYWORD_INT, TK_KEYWORD_FLOAT});
    auto &ident = read();
    ident.must({TK_IDENT});
    auto result = std::make_shared<FuncDef>(functype, ident);
    read().must({TK_LPAREN});
    if (peek().expect({TK_RPAREN})) {
        read();
    } else {
        result->params.emplace_back(ParseFuncFParam());
        while (peek().expect({TK_COMMA})) {
            read();
            result->params.emplace_back(ParseFuncFParam());
        }
        read().must({TK_RPAREN});
    }
    result->block = ParseBlock();
    return result;
}

// Stmt ->
std::shared_ptr<Stmt> Parser::ParseStmt() {
    auto result = std::make_shared<Stmt>();
    auto &token = peek();
    if (token.expect({TK_KEYWORD_WHILE})) {
        result->stmt = ParseWhileStmt();
    } else if (token.expect({TK_KEYWORD_IF})) {
        result->stmt = ParseIfStmt();
    } else if (token.expect({TK_KEYWORD_BREAK, TK_KEYWORD_CONTINUE})) {
        result->stmt = ParseCtrlStmt();
    } else if (token.expect({TK_KEYWORD_RETURN})) {
        result->stmt = ParseReturnStmt();
    } else if (token.expect({TK_LBRACK})) {
        result->stmt = ParseBlock();
    } else if (token.expect({TK_SEMI})){
        read();
    } else {
        for (int i = 1; ; i++) {
            auto &flag = peek(i);
            if (flag.expect({TK_ASSIGN})) {
                result->stmt = ParseAssignStmt();
                break;
            } else if (flag.expect({TK_SEMI})){
                result->stmt = ParseExp();
                read().must({TK_SEMI});
                break;
            }
        }
    }
    return result;
}

void Parser::output_ast(std::ostream &out) const {
    comp_unit->print(out, "", false);
}
