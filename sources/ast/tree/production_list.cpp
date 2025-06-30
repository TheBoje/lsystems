#include "ast/tree/production_list.h"

namespace ast {
production_list::production_list(std::vector<node*> nodes)
	: node_list(nodes) { }

production_list::production_list(production_list&& other)
	: node_list(std::move(other)) { }

production_list::production_list(const production_list& other)
	: node_list(other) { }

void production_list::print(std::ostream& stream) const {
	if (_nodes.empty()) {
		return;
	}

	for (size_t i = 0; i < _nodes.size(); i++) {
		stream << std::endl << "\t" << *_nodes.at(i);
	}
}
} // namespace ast
