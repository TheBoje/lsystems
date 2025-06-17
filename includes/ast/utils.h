#pragma once

#include "renderer/vulkan_renderer.h"

#include <string>
#include <vector>
#include <utility>

namespace ast {
class configuration;
class production_node;
class context_node;
} // namespace ast

namespace ast::utils {

std::pair<std::vector<renderer::vertex>, std::vector<uint32_t>> vertices_from_lsystem(ast::configuration* config, const std::vector<ast::production_node*> vAst);
std::string derive_lsystem(ast::configuration* config, const std::vector<ast::production_node*> vAst);

// Context matching
bool has_context(const ast::context_node* context);
bool is_context_match(const std::string& current, const ast::context_node* context);

// Random selection production
bool has_other_same_nodes(const std::vector<ast::production_node*> vAst, const ast::production_node* node);
bool is_same_matching_node(const ast::production_node& lhs, const ast::production_node& rhs);
std::string pick_random_replacement(const std::vector<ast::production_node*> vAst, const ast::production_node* node);

} // namespace ast::utils
