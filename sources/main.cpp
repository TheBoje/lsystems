#include "parser.tab.h"

#include "ast/program.h"
#include "semantic_analysis/semantic_analysis.h"

#include <iostream>

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  ast::program *program = nullptr;
  int res = yyparse(&program);
  (void)res;

  std::cout << "----- PARSING DONE -----" << std::endl;

  // semantic_analysis::SemanticAnalysis semanticAnalysis;
  // program->accept(semanticAnalysis);

  return 0;
}
