#include "frontend/lexer.h"
#include "frontend/parser.h"
#include "utils/error.h"
#include <iostream>
#include <fstream>

#define SOURCE_FILE "../resources/ast_demo.cpp"
#define TOKEN_LOG "../log/token.txt"
#define AST_LOG "../log/ast.txt"

int main() {
    std::ifstream srcfile(SOURCE_FILE);
    if (!srcfile.is_open()) {
        critical_error("Failed to open file " SOURCE_FILE);
    }
    Parser parser(make_token(srcfile));
    std::cout << "Token count : " << parser.token_list.token_list.size() << std::endl;
    std::ofstream token_out(TOKEN_LOG);
    if (!token_out.is_open()) {
        critical_error("Failed to open file " TOKEN_LOG);
    }
    output_token(parser.token_list, token_out);
    parser.parse();
    std::ofstream ast_out(AST_LOG);
    if (!ast_out.is_open()) {
        critical_error("Failed to open file " TOKEN_LOG);
    }
    parser.output_ast(ast_out);
    return 0;
}
