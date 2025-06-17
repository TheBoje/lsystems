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
bool has_context(const ast::context_node* context);
bool is_context_match(const std::string& current, const ast::context_node* context);

} // namespace ast::utils
