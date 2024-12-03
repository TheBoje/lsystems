#pragma once

#include <string>
#include <vector>

namespace ast {
class configuration;
class production_node;
class context_node;
} // namespace ast

namespace ast::utils {
std::string derive_lsystem(const ast::configuration* config, const std::vector<ast::production_node*> vAst);
bool has_context(const ast::context_node* context);
bool is_context_match(const std::string& current, const ast::context_node* context);
} // namespace ast::utils
