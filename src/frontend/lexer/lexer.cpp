//
// Created by zc175 on 2023/9/29.
//
#include "frontend/lexer.h"
#include "utils/error.h"
#include <iostream>
#include <regex>

struct MatchRule {
  std::regex pattern;
  TOKEN_TYPE token_type;
};

// Distinguish general identifiers and keywords
void TokenList::process_token(const std::string &raw, TOKEN_TYPE token_type,
                              size_t line, size_t column) {
  // Not an identifier at all
  if (token_type != TK_IDENT) {
    push({raw, token_type, line, column});
    return;
  }
  // Dictionary order should be identical to TOKEN_TYPE enum as token_type
  // recognition depends on it
  std::string dict[] = {"void", "int",   "float",    "const", "if",
                        "else", "while", "continue", "break", "return"};
  for (int i = 0; i < sizeof(dict) / sizeof(std::string); i++) {
    if (raw == dict[i]) {
      token_type = (TOKEN_TYPE)i;
      break;
    }
  }
  push({raw, token_type, line, column});
}

#define RULE(PATTERN, TOKEN_TYPE)                                              \
  { std::regex(PATTERN), TOKEN_TYPE }

void TokenList::make_token(std::istream &file) {
  MatchRule rule_table[] = {
      RULE("\\/\\/", TK_COMMENT),
      RULE("\\/\\*", TK_MLCOMMENT),
      RULE("\\*\\/", TK_MLCOMMENTEND),
      RULE(" +", TK_BLANK),
      RULE("[_a-zA-Z]+[_a-zA-Z0-9]*", TK_IDENT),
      RULE(",", TK_COMMA),
      RULE(";", TK_SEMI),
      RULE("\\(", TK_LPAREN),
      RULE("\\)", TK_RPAREN),
      RULE("\\[", TK_LBRACE),
      RULE("]", TK_RBRACE),
      RULE("\\{", TK_LBRACK),
      RULE("}", TK_RBRACK),
      // // float rule is so complicated
      // // that I have to split it
      // // Rule for decimal fractional-constant exponent-part(opt)
      RULE("(?:[0-9]*\\.[0-9]+|[0-9]+\\.)(?:[eE][+-]?[0-9]+)?|"
           // // Rule for decimal digit-sequence exponent-part
           "[0-9]+[eE][+-]?[0-9]+|"
           // // Rule for hexadecimal-float
           "0[xX](?:[0-9A-Fa-f]+\\.?[0-9A-Fa-f]*|\\.[0-9A-Fa-f]+)[pP][-+]?[0-9]"
           "+",
           TK_CONST_FLOAT),
      RULE("0[xX][0-9A-Fa-f]+|0[0-7]*|[1-9]+[0-9]*", TK_CONST_INT),
      RULE("\\+", TK_OP_ADD),
      RULE("-", TK_OP_SUB),
      RULE("\\*", TK_OP_MUL),
      RULE("%", TK_OP_MOD),
      RULE("\\/", TK_OP_DIV),
      RULE("<=", TK_OP_LE),
      RULE(">=", TK_OP_GE),
      RULE("==", TK_OP_EQ),
      RULE("!=", TK_OP_NE),
      RULE("<", TK_OP_LT),
      RULE(">", TK_OP_GT),
      RULE("!", TK_OP_NOT),
      RULE("&&", TK_OP_AND),
      RULE("\\|\\|", TK_OP_OR),
      RULE("=", TK_ASSIGN),
  };
  std::string temp;
  size_t line = 1;
  bool in_comment = false;
  while (std::getline(file, temp)) {
    // match a line
    size_t pos = 1;
    // Append CR to the last line
    if (temp[temp.size() - 1] != (char)13) {
      temp += (char)13;
    }
    while (pos < temp.size()) {
      // match part of a line
      bool valid = false;
      std::string substr = temp.substr(pos - 1, temp.size() - pos + 1);
      if (in_comment) {
        std::smatch match_result;
        if (std::regex_search(substr, match_result, rule_table[2].pattern)) {
          pos += 2 + match_result.position();
          substr = temp.substr(pos - 1, temp.size() - pos + 1);
          in_comment = false;
        } else {
          break;
        }
      }
      for (auto &i : rule_table) {
        std::smatch match_result;
        if (std::regex_search(substr, match_result, i.pattern)) {
          // Must match the first char
          if (match_result.position() != 0)
            continue;
          valid = true;
          // comment
          if (i.token_type == TK_COMMENT) {
            pos += substr.size();
            break;
          } else if (i.token_type == TK_MLCOMMENT) {
            in_comment = true;
            break;
          } else if (i.token_type == TK_MLCOMMENTEND) {
            Token error_token = {.line = line, .column = pos};
            critical_error(error_token, "Unexpected comment end.");
          }
          // Ignore spaces
          if (i.token_type != TK_BLANK)
            // Process Identifier
            process_token(match_result.str(), i.token_type, line, pos);
          pos += match_result.str().size();
          break;
        }
      }
      // Invalid token
      if (!valid) {
        Token error_token = {.line = line, .column = pos};
        critical_error(error_token, "Unrecognized token");
      }
    }
    line++;
  }
}

void TokenList::output_token(std::ostream &file) {
  for (const auto &each : list) {
    file << each.raw << " " << each.line << " " << each.column << " "
         << each.token_type << std::endl;
  }
}
