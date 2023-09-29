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

void Token::must(TOKEN_TYPE type) {
    if (this->token_type != type) {
        critical_error(*this, "Expected token type %d", type);
    }
}

bool Token::expect(TOKEN_TYPE type) const {
    if (this->token_type != type) return false;
    return true;
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

Token& TokenList::peek() {
    return this->peek(0);
}

Token& TokenList::peek(size_t offset) {
    if (this->pos + offset == token_list.size()) {
        critical_error("Unexpected file end");
    }
    return this->token_list[this->pos + offset];
}