#include "ast/tree/definition_list.h"

namespace ast {

definition_list::definition_list(std::vector<node*> nodes)
	: node_list(nodes) { }

definition_list::definition_list(definition_list&& other)
	: node_list(std::move(other)) { }

definition_list::definition_list(const definition_list& other)
	: node_list(other) { }

void definition_list::print(std::ostream& stream) const {
	if (_nodes.empty()) {
		return;
	}

	for (auto node : _nodes) {
		stream << std::endl << "\t" << *node;
	}
}

} // namespace ast
