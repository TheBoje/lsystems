
#include "ast/ast.h"
#include "ast/configuration.h"

#include "parser.tab.h"

#include <iostream>
#include <vector>

extern int yyparse();
extern std::vector<ast::production_node *> vAst;
extern ast::configuration *config;

std::string derive_lsystem() {
  std::string result = config->axiom;

  for (int step = 0; step < config->derivation; ++step) {
    std::string new_result = "";

    for (char c : result) {
      bool matched = false;
      for (size_t i = 0; i < vAst.size(); i++) {
        if (vAst[i]->symbol_node->symbol[0] == c) {
          new_result += vAst[i]->replacement_node->result_symbol;
          matched = true;
          break;
        }
      }
      if (!matched) {
        new_result += c;
      }
    }

    result = new_result;
  }

  return result;
}

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  yyparse();

  if (vAst.size() <= 0) {
    std::cout << "nodes: none" << std::endl;
  }

  for (ast::node *node : vAst) {
    std::cout << "node: " << *node << std::endl;
  }

  std::cout << "config: " << std::endl << *config << std::endl;

  std::cout << derive_lsystem() << std::endl;

  return 0;
}
