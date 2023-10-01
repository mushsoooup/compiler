//
// Created by zc175 on 2023/9/29.
//
#include "frontend/ast.h"

static void print_prefix(std::ostream &out, const std::string &prefix, bool isLeft) {
    out << prefix;
    out << (isLeft ? std::string("├──") : std::string("└──"));
}

void Number::print(std::ostream& out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("Number : " + token.raw + "\n");
}

void LVal::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("LVal : " + token.raw + "\n");
}

void ParenExp::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("ParenExp\n");
    exp->print(out,  prefix + (isLeft ? "│   " : "    "), true);
}

void Exp::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("Exp\n");
    exp->print(out,  prefix + (isLeft ? "│   " : "    "), true);
}

void AddExp::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("AddExp\n");
    exps.at(0)->print(out,  prefix + (isLeft ? "│   " : "    "), true);
    for (int i = 0; i < ops.size(); i++) {
        ops.at(i)->print(out,  prefix + (isLeft ? "│   " : "    "), false);
        exps.at(i + 1)->print(out,  prefix + (isLeft ? "│   " : "    "), false);
    }
}

void RelExp::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("RelExp\n");
    exps.at(0)->print(out,  prefix + (isLeft ? "│   " : "    "), true);
    for (int i = 0; i < ops.size(); i++) {
        ops.at(i)->print(out,  prefix + (isLeft ? "│   " : "    "), false);
        exps.at(i + 1)->print(out,  prefix + (isLeft ? "│   " : "    "), false);
    }
}

void EqExp::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("RelExp\n");
    exps.at(0)->print(out,  prefix + (isLeft ? "│   " : "    "), true);
    for (int i = 0; i < ops.size(); i++) {
        ops.at(i)->print(out,  prefix + (isLeft ? "│   " : "    "), false);
        exps.at(i + 1)->print(out,  prefix + (isLeft ? "│   " : "    "), false);
    }
}

void LAndExp::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("RelExp\n");
    exps.at(0)->print(out,  prefix + (isLeft ? "│   " : "    "), true);
    for (int i = 1; i < exps.size(); i++) {
        exps.at(i)->print(out,  prefix + (isLeft ? "│   " : "    "), false);
    }
}

void LOrExp::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("RelExp\n");
    exps.at(0)->print(out,  prefix + (isLeft ? "│   " : "    "), true);
    for (int i = 1; i < exps.size(); i++) {
        exps.at(i)->print(out,  prefix + (isLeft ? "│   " : "    "), false);
    }
}

void MulExp::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("MulExp\n");
    exps.at(0)->print(out,  prefix + (isLeft ? "│   " : "    "), true);
    for (int i = 0; i < ops.size(); i++) {
        ops.at(i)->print(out,  prefix + (isLeft ? "│   " : "    "), false);
        exps.at(i + 1)->print(out,  prefix + (isLeft ? "│   " : "    "), false);
    }
}

void UnaryExp::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("UnaryExp\n");
    auto visitor = [&](auto &arg) {
        arg->print(out,  prefix + (isLeft ? "│   " : "    "), true);
    };
    std::visit(visitor, exp);
}

void PrimaryExp::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("PrimaryExp\n");
    auto visitor = [&](auto &arg) {
        arg->print(out,  prefix + (isLeft ? "│   " : "    "), true);
    };
    std::visit(visitor, exp);
}

void Callee::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("Callee : " + token.raw + "\n");
    if (!params.empty()) {
        params.at(0)->print(out, prefix + (isLeft ? "│   " : "    "), true);
        for (int i = 1; i < params.size(); i++) {
            params.at(i)->print(out, prefix + (isLeft ? "│   " : "    "), false);
        }
    }
}

void UnaryOp::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("UnaryOp : " + op.raw + "\n");
    exp->print(out,  prefix + (isLeft ? "│   " : "    "), true);
}

void VarDef::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("VarDef : " + ident.raw);
    // TODO: output dimension info
    out << std::string("\n");
    exp->print(out,  prefix + (isLeft ? "│   " : "    "), true);
}

void VarDecl::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("ValDecl : " + std::string(is_const ? "const " : "") + btype.raw + "\n");
    defs.at(0)->print(out,  prefix + (isLeft ? "│   " : "    "), true);
    for (int i = 1; i < defs.size(); i++) {
        defs.at(i)->print(out,  prefix + (isLeft ? "│   " : "    "), false);
    }
}

void FuncFParam::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("FuncFParam : " + btype.raw + " " + ident.raw + "\n");
}

void Cond::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("Cond\n");
    exp->print(out,  prefix + (isLeft ? "│   " : "    "), true);
}

void AssignStmt::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("AssignStmt\n");
    lval->print(out,  prefix + (isLeft ? "│   " : "    "), true);
    exp->print(out,  prefix + (isLeft ? "│   " : "    "), false);
}

void IfStmt::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("IfStmt\n");
    cond->print(out, prefix + (isLeft ? "│   " : "    "), true);
    body->print(out, prefix + (isLeft ? "│   " : "    "), false);
    if (else_body != nullptr) {
        else_body->print(out, prefix + (isLeft ? "│   " : "    "), false);
    }
}

void WhileStmt::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("WhileStmt\n");
    cond->print(out, prefix + (isLeft ? "│   " : "    "), true);
    body->print(out, prefix + (isLeft ? "│   " : "    "), false);
}

void CtrlStmt::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("CtrlStmt " + std::string(type ? "break" : "continue"));
}

void ReturnStmt::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("ReturnStmt\n");
    exp->print(out, prefix + (isLeft ? "│   " : "    "), true);
}

void Block::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("Block\n");
    auto visitor = [&](auto &arg, bool _isLeft) {
        arg->print(out, prefix + (isLeft ? "│   " : "    "), _isLeft);
    };
    std::visit([&](auto &arg) { visitor(arg, false); }, items.at(0));
    for (int i = 1; i < items.size(); i++) {
        std::visit([&](auto &arg) { visitor(arg, true); }, items.at(1));
    }
}

void FuncDef::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("FuncDef : " + functype.raw + " " + ident.raw + "\n");
    if (!params.empty()) {
        params.at(0)->print(out, prefix + (isLeft ? "│   " : "    "), true);
        for (int i = 1; i < params.size(); i++) {
            params.at(i)->print(out, prefix + (isLeft ? "│   " : "    "), false);
        }
    }
    block->print(out, prefix + (isLeft ? "│   " : "    "), params.empty());
}

void Stmt::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("Stmt\n");
    auto visitor = [&](auto &arg) {
        arg->print(out, prefix + (isLeft ? "│   " : "    "), true);
    };
    std::visit(visitor, stmt);
}

void CompUnit::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("CompUnit") << std::string("\n");
    auto visitor = [&](auto &arg, bool _isLeft) {
        arg->print(out,  prefix + (isLeft ? "│   " : "    "), _isLeft);
    };
    std::visit([&](auto &arg) { visitor(arg, false); }, decls.at(0));
    for (int i = 1; i < decls.size(); i++) {
        std::visit([&](auto &arg) { visitor(arg, true); }, decls.at(i));
    }
}
