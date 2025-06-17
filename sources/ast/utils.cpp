#include "ast/utils.h"

#include "ast/ast.h"
#include "ast/configuration.h"

#include <cassert>
#include <utility>
#include <stack>

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <glm/trigonometric.hpp>

namespace ast::utils {

std::pair<std::vector<renderer::vertex>, std::vector<uint32_t>> vertices_from_lsystem(ast::configuration* config, const std::vector<ast::production_node*> vAst) {
	if (config->result.empty()) {
		derive_lsystem(config, vAst);
	}

	assert(config->result.empty() == false);

	struct State {
		glm::vec3 pos;
		glm::vec3 dir_forward;
		glm::vec3 dir_left;
		glm::vec3 dir_up;
		uint32_t index;
	};

	std::vector<renderer::vertex> vertices;
	std::vector<uint32_t> indices;
	const auto angle = glm::radians(config->angle_factor);

	State state;
	state.pos = {0.0f, 0.0f, -1.0f};
	state.dir_forward = {0.0f, 0.0f, 1.0f};
	state.dir_left = {1.0f, 0.0f, 0.0f};
	state.dir_up = {0.0f, 1.0f, 0.0f};
	state.index = 0;

	std::stack<State> stack;

	const glm::vec3 color = {1.0f, 1.0f, 1.0f};
	vertices.push_back({state.pos, color});

	for (const char current_char : config->result) {
		switch (current_char) {
			case 'F': {
				glm::vec3 next = state.pos + state.dir_forward * config->scale_factor;

				vertices.push_back({next, color});
				uint32_t newIndex = vertices.size() - 1;

				indices.push_back(state.index);
				indices.push_back(newIndex);

				state.index = newIndex;
				state.pos = next;

				break;
			}
			case '+': { // yaw left
				state.dir_forward = glm::rotate(state.dir_forward, angle, state.dir_up);
				state.dir_left = glm::rotate(state.dir_left, angle, state.dir_up);
				break;
			}
			case '-': { // yaw right
				state.dir_forward = glm::rotate(state.dir_forward, -angle, state.dir_up);
				state.dir_left = glm::rotate(state.dir_left, -angle, state.dir_up);
				break;
			}
			case '&': { // pitch down
				state.dir_forward = glm::rotate(state.dir_forward, angle, state.dir_left);
				state.dir_up = glm::rotate(state.dir_up, angle, state.dir_left);
				break;
			}
			case '^': { // pitch up
				state.dir_forward = glm::rotate(state.dir_forward, -angle, state.dir_left);
				state.dir_up = glm::rotate(state.dir_up, -angle, state.dir_left);
				break;
			}
			case '\\': { // roll left
				state.dir_left = glm::rotate(state.dir_left, angle, state.dir_forward);
				state.dir_up = glm::rotate(state.dir_up, angle, state.dir_forward);
				break;
			}
			case '/': { // roll right
				state.dir_left = glm::rotate(state.dir_left, -angle, state.dir_forward);
				state.dir_up = glm::rotate(state.dir_up, -angle, state.dir_forward);
				break;
			}
			case '|': { // reverse
				state.dir_forward = -state.dir_forward;
				state.dir_left = -state.dir_left;
				// No -up ?
				break;
			}
			case '[': {
				stack.push(state);
				break;
			}
			case ']': {
				state = stack.top();
				stack.pop();

				vertices.push_back({state.pos, color});
				state.index = vertices.size() - 1;

				break;
			}
			default: {
				// nothing!
				break;
			}
		}
	}

	return {vertices, indices};
}

std::string derive_lsystem(ast::configuration* config, const std::vector<ast::production_node*> vAst) {
	std::string result = config->axiom;

	std::cout << "derivation 0/" << config->derivation << ": " << result << std::endl;

	for (int step = 0; step < config->derivation; ++step) {
		std::string new_result = "";

		for (size_t i = 0; i < result.size(); i++) {
			const char c = result[i];
			bool matched = false;
			for (size_t j = 0; j < vAst.size(); j++) {
				if (vAst[j]->symbol_node->symbol[0] == c) {
					if (has_context(vAst[j]->left_context)) {
						const std::string left_subset(result.begin() + i - vAst[j]->left_context->context_symbols.size(), result.begin() + i);
						if (!is_context_match(left_subset, vAst[j]->left_context)) {
							continue;
						}
					}

					if (has_context(vAst[j]->right_context)) {
						const std::string right_subset(result.begin() + i + 1, result.begin() + i + 1 + vAst[j]->right_context->context_symbols.size());
						if (!is_context_match(right_subset, vAst[j]->right_context)) {
							continue;
						}
					}
					new_result += vAst[j]->replacement_node->result_symbol->symbol;
					matched = true;
					break;
				}
			}
			if (!matched) {
				new_result += c;
			}
		}

		result = new_result;
		std::cout << "derivation " << (step + 1) << "/" << config->derivation << ": " << result << std::endl;
	}

	config->result = result;

	return result;
}

bool has_context(const context_node* context) {
	if (!context) {
		return false;
	}
	return context->context_symbols != "*";
}

bool is_context_match(const std::string& current, const ast::context_node* context) {
	assert(context);
	return current == context->context_symbols;
}

} // namespace ast::utils
