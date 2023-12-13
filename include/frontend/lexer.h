//
// Created by zc175 on 2023/9/29.
//
#pragma once
#include <string>
#include <vector>

enum TOKEN_TYPE {
  // keywords
  // void int float const
  TK_KEYWORD_VOID,
  TK_KEYWORD_INT,
  TK_KEYWORD_FLOAT,
  TK_KEYWORD_CONST,
  // if else
  TK_KEYWORD_IF,
  TK_KEYWORD_ELSE,
  // while continue break
  TK_KEYWORD_WHILE,
  TK_KEYWORD_CONTINUE,
  TK_KEYWORD_BREAK,
  // return
  TK_KEYWORD_RETURN,
  // identifier
  TK_IDENT,
  // operators
  // + -
  TK_OP_ADD,
  TK_OP_SUB,
  // * / %
  TK_OP_MUL,
  TK_OP_DIV,
  TK_OP_MOD,
  // == !=
  TK_OP_EQ,
  TK_OP_NE,
  // < > <= >=
  TK_OP_LT,
  TK_OP_GT,
  TK_OP_LE,
  TK_OP_GE,
  // ! && ||
  TK_OP_NOT,
  TK_OP_AND,
  TK_OP_OR,
  // characters
  TK_BLANK,
  TK_COMMENT,
  TK_MLCOMMENT,
  TK_MLCOMMENTEND,
  // =
  TK_ASSIGN,
  // , ;
  TK_COMMA,
  TK_SEMI,
  // () [] {}
  TK_LPAREN,
  TK_RPAREN,
  TK_LBRACE,
  TK_RBRACE,
  TK_LBRACK,
  TK_RBRACK,
  // consts
  // float, int constants
  TK_CONST_FLOAT,
  TK_CONST_INT,
};

class Token {
public:
  // Matched token string
  std::string raw;
  // Token tyep
  TOKEN_TYPE token_type;
  // Token position
  size_t line, column;
  [[nodiscard]] bool is_keyword() const;
  [[nodiscard]] bool is_type() const;
  // Calls exit if token_type is unexpected
  void must(std::initializer_list<TOKEN_TYPE>);
  // Check if token_type is unexpected
  [[nodiscard]] bool expect(std::initializer_list<TOKEN_TYPE>) const;
  void print(std::ostream &out, const std::string &prefix, bool isLeft) const;
};

class TokenList {
  std::vector<Token> list;
  size_t pos = 0;
  void process_token(const std::string &raw, TOKEN_TYPE token_type, size_t line,
                     size_t column);

public:
  [[nodiscard]] size_t size() { return list.size(); };
  void push(const Token &);
  Token &read();
  [[nodiscard]] Token &peek(size_t offset);
  [[nodiscard]] bool empty() const;
  void make_token(std::istream &);
  void output_token(std::ostream &);
};
