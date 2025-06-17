#pragma once

#include "renderer/vulkan_renderer.h"

#include <string>
#include <vector>
#include <utility>

namespace ast {
class configuration;
class production;
class context;
} // namespace ast

namespace ast::utils {

std::pair<std::vector<renderer::vertex>, std::vector<uint32_t>> vertices_from_lsystem(ast::configuration* config, const std::vector<ast::production*> vAst);
std::string derive_lsystem(ast::configuration* config, const std::vector<ast::production*> vAst);

// Context matching
bool has_context(const ast::context* context);
bool is_context_match(const std::string& current, const ast::context* context);

// Random selection production
bool has_other_same_nodes(const std::vector<ast::production*> vAst, const ast::production* node);
bool is_same_matching_node(const ast::production& lhs, const ast::production& rhs);
std::string pick_random_replacement(const std::vector<ast::production*> vAst, const ast::production* node);

} // namespace ast::utils
