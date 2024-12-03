#include "ast/utils.h"

#include "ast/ast.h"
#include "ast/configuration.h"
#include <algorithm>
#include <cassert>
#include <cstddef>

namespace ast::utils {

std::string derive_lsystem(const ast::configuration* config, const std::vector<ast::production_node*> vAst) {
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
