//
// Created by zc175 on 2023/9/29.
//
#include "frontend/parser.h"
#include "frontend/ast.h"

static void print_prefix(std::ostream &out, const std::string &prefix, bool isLeft) {
    out << prefix;
    out << (isLeft ? std::string("├──") : std::string("└──"));
}

void Number::print(std::ostream& out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("Number : ") << token.raw << std::string("\n");
}

void LVal::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("LVal : ") << token.raw << std::string("\n");
}

void ParenExp::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("ParenExp") << std::string("\n");
    exp->print(out,  prefix + (isLeft ? "│   " : "    "), true);
}

void Exp::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("Exp") << std::string("\n");
    exp->print(out,  prefix + (isLeft ? "│   " : "    "), true);
}

void AddExp::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("AddExp") << std::string("\n");
    exps.at(0)->print(out,  prefix + (isLeft ? "│   " : "    "), true);
    for (int i = 0; i < ops.size(); i++) {
        ops.at(i)->print(out,  prefix + (isLeft ? "│   " : "    "), false);
        exps.at(i + 1)->print(out,  prefix + (isLeft ? "│   " : "    "), false);
    }
}

void MulExp::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("MulExp") << std::string("\n");
    exps.at(0)->print(out,  prefix + (isLeft ? "│   " : "    "), true);
    for (int i = 0; i < ops.size(); i++) {
        ops.at(i)->print(out,  prefix + (isLeft ? "│   " : "    "), false);
        exps.at(i + 1)->print(out,  prefix + (isLeft ? "│   " : "    "), false);
    }
}

void UnaryExp::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("UnaryExp") << std::string("\n");
    auto visitor = [&](auto &arg) {
        arg->print(out,  prefix + (isLeft ? "│   " : "    "), true);
    };
    std::visit(visitor, exp);
}

void PrimaryExp::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("PrimaryExp") << std::string("\n");
    auto visitor = [&](auto &arg) {
        arg->print(out,  prefix + (isLeft ? "│   " : "    "), true);
    };
    std::visit(visitor, exp);
}

void Callee::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("Callee : ") << token.raw << std::string("()\n");
}

void UnaryOp::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("UnaryOp : ") << op.raw << std::string("\n");
    exp->print(out,  prefix + (isLeft ? "│   " : "    "), true);
}

void VarDef::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("VarDef : ") << ident.raw;
    // TODO: output dimension info
    out << std::string("\n");
    exp->print(out,  prefix + (isLeft ? "│   " : "    "), true);
}

void VarDecl::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("ValDecl : ") << btype.raw << std::string("\n");
    defs.at(0)->print(out,  prefix + (isLeft ? "│   " : "    "), true);
    for (int i = 1; i < defs.size(); i++) {
        defs.at(i)->print(out,  prefix + (isLeft ? "│   " : "    "), false);
    }
}

void CompUnit::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    print_prefix(out, prefix, isLeft);
    out << std::string("CompUnit") << std::string("\n");
    auto visitor = [&](auto &arg) {
        arg->print(out,  prefix + (isLeft ? "│   " : "    "), true);
    };
    auto visitor2 = [&](auto &arg) {
        arg->print(out,  prefix + (isLeft ? "│   " : "    "), false);
    };
    std::visit(visitor, decls.at(0));
    for (int i = 1; i < decls.size(); i++) {
        std::visit(visitor2, decls.at(i));
    }
}
