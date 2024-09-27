#include "ast/utils.h"

#include "ast/ast.h"
#include "ast/configuration.h"

namespace ast::utils {

std::string derive_lsystem(const ast::configuration* config, const std::vector<ast::production_node*> vAst) {
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
} // namespace ast::utils
