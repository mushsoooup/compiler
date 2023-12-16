#include "frontend/lexer.h"
#include "frontend/parser.h"
#include "utils/error.h"
#include <fstream>
#include <iostream>
#include <string>

#define SOURCE_FILE "../resources/ast_demo.cpp"
#define TOKEN_LOG "../log/token.txt"
#define AST_LOG "../log/ast.txt"

int main(int argc, char **argv) {
  std::string source = SOURCE_FILE;
  if (argc == 2) {
    source = argv[1];
  }
  std::ifstream srcfile(SOURCE_FILE);
  if (!srcfile.is_open()) {
    critical_error("Failed to open file " SOURCE_FILE);
  }

  TokenList tokens;
  tokens.make_token(srcfile);
  std::cout << "Token count : " << tokens.size() << std::endl;

  Parser parser(tokens);

  std::ofstream token_out(TOKEN_LOG);
  if (!token_out.is_open()) {
    critical_error("Failed to open file " TOKEN_LOG);
  }
  tokens.output_token(token_out);
  parser.parse();
  std::ofstream ast_out(AST_LOG);
  if (!ast_out.is_open()) {
    critical_error("Failed to open file " TOKEN_LOG);
  }
  parser.output_ast(ast_out);
  // TODO: Semantic analysis
  // Type check
  // Initialize global variables to zero;
  return 0;
}
