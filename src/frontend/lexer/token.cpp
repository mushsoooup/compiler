//
// Created by zc175 on 2023/9/29.
//
#include "frontend/lexer.h"
#include "utils/error.h"

bool Token::is_keyword() const {
    return this->token_type >= TK_KEYWORD_VOID && this->token_type <= TK_KEYWORD_RETURN;
}

bool Token::is_type() const {
    return this->token_type >=TK_KEYWORD_VOID && this->token_type <= TK_KEYWORD_CONST;
}

void Token::must(std::initializer_list<TOKEN_TYPE> types) {
    if (!expect(types)) {
        critical_error(*this, "Unexpected token type");
    }
}

bool Token::expect(std::initializer_list<TOKEN_TYPE> types) const {
    bool flag = false;
    for (auto type : types) {
        if (type == token_type) {
            flag = true;
            break;
        }
    }
    return flag;
}

void Token::print(std::ostream &out, const std::string &prefix, bool isLeft) const {
    out << prefix;
    out << (isLeft ? std::string("├──") : std::string("└──"));
    out << raw << std::string("\n");
}

void TokenList::push(const Token& tk) {
    this->token_list.push_back(tk);
}

Token& TokenList::read() {
    if (this->pos == token_list.size()) {
        critical_error("Unexpected file end");
    }
    return this->token_list[this->pos++];
}

Token& TokenList::peek(size_t offset) {
    if (this->pos + offset == token_list.size()) {
        critical_error("Unexpected file end");
    }
    return this->token_list[this->pos + offset];
}

bool TokenList::empty() const {
    return pos == token_list.size();
}
