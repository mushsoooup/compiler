//
// Created by zc175 on 2023/9/29.
//
#include "frontend/lexer.h"
#include "utils/error.h"
#include <regex>
#include <iostream>

struct MatchRule {
    std::regex pattern;
    TOKEN_TYPE token_type;
};


// Distinguish general identifiers and keywords
static void process_token(TokenList &token_list, const std::string& raw, TOKEN_TYPE token_type, size_t line, size_t column) {
    // Not an identifier at all
    if (token_type != TK_IDENT) {
        token_list.push({raw, token_type, line, column});
        return;
    }
    // Dictionary order should be identical to TOKEN_TYPE enum as token_type recognition depends on it
    std::string dict[] = {"void", "int", "float", "const", "if", "else", "while", "continue", "break", "return"};
    for(int i = 0; i < sizeof(dict) / sizeof(std::string); i++) {
        if (raw == dict[i]) {
            token_type = (TOKEN_TYPE)i;
            break;
        }
    }
    token_list.push({raw, token_type, line, column});
}

#define RULE(PATTERN, TOKEN_TYPE) {std::regex(PATTERN), TOKEN_TYPE}

TokenList make_token(std::istream& file) {
    TokenList token_list;
    MatchRule rule_table[] = {
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
            RULE("(?:[0-9]?\\.[0-9]+|[0-9]+\\.)(?:[eE][+-]?[0-9]+)?|"
                 // // Rule for decimal digit-sequence exponent-part
                 "[0-9]+[eE][+-]?[0-9]+|"
                 // // Rule for hexadecimal-float
                 "0[xX](?:[0-9A-Fa-f]+\\.?[0-9A-Fa-f]*|\\.[0-9A-Fa-f]+)[pP][-+]?[0-9]+", TK_CONST_FLOAT),
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
    while(std::getline(file, temp)) {
        // match a line
        size_t pos = 1;
        // Append CR to the last line
        if (temp[temp.size() - 1] != (char)13) {
            temp += (char)13;
        }
        while(pos < temp.size()) {
            // match part of a line
            bool valid = false;
            std::string substr = temp.substr(pos - 1, temp.size() - pos + 1);
            for(auto & i : rule_table) {
                std::smatch match_result;
                if (std::regex_search(substr, match_result, i.pattern)) {
                    // Must match the first char
                    if (match_result.position() != 0) continue;
                    valid = true;
                    // Ignore spaces
                    if (i.token_type != TK_BLANK)
                        // Process Identifier
                        process_token(token_list, match_result.str(), i.token_type, line, pos);
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
    return token_list;
}

void output_token(TokenList &token_list, std::ostream &file) {
    for(const auto& each : token_list.token_list) {
        file << each.raw << " " << each.line << " " << each.column << std::endl;
    }
}