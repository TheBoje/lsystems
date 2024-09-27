#pragma once

#include <string>
#include <vector>

namespace ast {
class configuration;
class production_node;
} // namespace ast

namespace ast::utils {
std::string derive_lsystem(const ast::configuration* config, const std::vector<ast::production_node*> vAst);
}
