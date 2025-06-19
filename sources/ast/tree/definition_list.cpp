#include "ast/tree/definition_list.h"

namespace ast {

definition_list::definition_list(std::vector<node*> nodes)
	: node_list(nodes) { }

void definition_list::print(std::ostream& stream) const {
	if (_nodes.empty()) {
		return;
	}

	for (auto node : _nodes) {
		stream << std::endl << "\t" << *node;
	}
}

} // namespace ast
