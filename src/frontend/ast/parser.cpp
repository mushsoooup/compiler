//
// Created by zc175 on 2023/9/29.
//
#include <memory>

#include "frontend/parser.h"

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
std::unique_ptr<CompUnit> Parser::ParseCompUnit() {
    auto result = std::make_unique<CompUnit>();
    while (!token_list.empty()) {
        if (peek(2).expect({TK_LPAREN})) {
        } else {
            result->decls.emplace_back(ParseVarDecl());
        }
    }
    return result;
}

// VarDecl -> BType VarDef (, VarDef)* ;
std::unique_ptr<VarDecl> Parser::ParseVarDecl() {
    auto &token = read();
    token.must({TK_KEYWORD_INT, TK_KEYWORD_FLOAT});
    auto result = std::make_unique<VarDecl>(token);
    result->defs.push_back(ParseVarDef());
    while (peek().expect({TK_COMMA})) {
        read();
        result->defs.push_back(ParseVarDef());
    }
    read().must({TK_SEMI});
    return result;
}

// VarDef -> ident (= Exp)?
std::unique_ptr<VarDef> Parser::ParseVarDef() {
    auto &token = read();
    token.must({TK_IDENT});
    auto result = std::make_unique<VarDef>(token);
    read().must({TK_ASSIGN});
    if (peek().expect({TK_COMMA, TK_SEMI})) {
        result->exp = nullptr;
    } else {
        result->exp = ParseExp();
    }
    return result;
}

// Exp -> AddExp
std::unique_ptr<Exp> Parser::ParseExp() {
    return std::make_unique<Exp>(ParseAddExp());
}

// MulExp -> UnaryExp (MulOp UnaryExp)*
// MulOp -> * | / | %
std::unique_ptr<MulExp> Parser::ParseMulExp() {
    auto result = std::make_unique<MulExp>();
    result->exps.push_back(ParseUnaryExp());
    while (peek().expect({TK_OP_MUL, TK_OP_DIV, TK_OP_MOD})) {
        result->ops.push_back(&read());
        result->exps.push_back(ParseUnaryExp());
    }
    return result;
}

// UnaryExp -> PrimaryExp | Callee | UnaryOp
std::unique_ptr<UnaryExp> Parser::ParseUnaryExp() {
    auto &token = peek();
    auto result = std::make_unique<UnaryExp>();
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
std::unique_ptr<PrimaryExp> Parser::ParsePrimaryExp() {
    auto result = std::make_unique<PrimaryExp>();
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
std::unique_ptr<Callee> Parser::ParseCallee() {
    auto &token = read();
    token.must({TK_IDENT});
    read().must({TK_LPAREN});
    read().must({TK_RPAREN});
    auto result = std::make_unique<Callee>(token);
    return result;
}

// UnaryOp -> UnaryOperator UnaryExp
// UnaryOperator -> + | - | !
std::unique_ptr<UnaryOp> Parser::ParseUnaryOp() {
    auto &token = read();
    token.must({TK_OP_ADD, TK_OP_SUB, TK_OP_NOT});
    auto result = std::make_unique<UnaryOp>(token);
    result->exp = ParseUnaryExp();
    return result;
}

// Number -> int | float
std::unique_ptr<Number> Parser::ParseNumber() {
    auto &token = read();
    token.must({TK_CONST_INT, TK_CONST_FLOAT});
    return std::make_unique<Number>(token);
}

// LVal -> ident
std::unique_ptr<LVal> Parser::ParseLVal() {
    auto &token = read();
    token.must({TK_IDENT});
    return std::make_unique<LVal>(token);
}

// ParenExp -> (Exp)
std::unique_ptr<ParenExp> Parser::ParseParenExp() {
    auto &token = read();
    token.must({TK_LPAREN});
    auto result = std::make_unique<ParenExp>(ParseExp());
    read();
    return result;
}

// AddExp -> MulExp (AddOp MulExp)*
// AddOp -> + | -
std::unique_ptr<AddExp> Parser::ParseAddExp() {
    auto result = std::make_unique<AddExp>();
    result->exps.push_back(ParseMulExp());
    while (peek().expect({TK_OP_ADD, TK_OP_SUB})) {
        result->ops.push_back(&read());
        result->exps.push_back(ParseMulExp());
    }
    return result;
}

// RelExp -> AddExp (RelOp AddExp)*
// RelOp -> < | > | <= | >=
std::unique_ptr<RelExp> Parser::ParseRelExp() {
    auto result = std::make_unique<RelExp>();
    result->exps.push_back(ParseAddExp());
    while (peek().expect({TK_OP_LT, TK_OP_GT, TK_OP_LE, TK_OP_GE})) {
        result->ops.push_back(&read());
        result->exps.push_back(ParseAddExp());
    }
    return result;
}

// EqExp -> RelExp (EqOp RelExp)*
// EqOp -> == | !=
std::unique_ptr<EqExp> Parser::ParseEqExp() {
    auto result = std::make_unique<EqExp>();
    result->exps.push_back(ParseRelExp());
    while (peek().expect({TK_OP_EQ, TK_OP_NE})) {
        result->ops.push_back(&read());
        result->exps.push_back(ParseRelExp());
    }
    return result;
}

// LAndExp -> EqExp (&& EqExp)*
std::unique_ptr<LAndExp> Parser::ParseLAndExp() {
    auto result = std::make_unique<LAndExp>();
    result->exps.push_back(ParseEqExp());
    while (peek().expect({TK_OP_AND})) {
        read();
        result->exps.push_back(ParseEqExp());
    }
    return result;
}

// LOrExp -> LAndExp (|| LAndExp)*
std::unique_ptr<LOrExp> Parser::ParseLOrExp() {
    auto result = std::make_unique<LOrExp>();
    result->exps.push_back(ParseLAndExp());
    while (peek().expect({TK_OP_AND})) {
        read();
        result->exps.push_back(ParseLAndExp());
    }
    return result;
}

void Parser::output_ast(std::ostream &out) const {
    comp_unit->print(out, "", false);
}
