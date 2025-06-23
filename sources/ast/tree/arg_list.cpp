#include "ast/tree/arg_list.h"

namespace ast {

arg_list::arg_list(std::vector<node*> nodes)
	: node_list(nodes) { }

arg_list::arg_list(arg_list&& other)
	: node_list(std::move(other)) { }

arg_list::arg_list(const arg_list& other)
	: node_list(other) { }

void arg_list::print(std::ostream& stream) const {
	stream << *_nodes.front();

	if (_nodes.size() == 1) {
		return;
	}

	for (size_t i = 1; i < _nodes.size(); i++) {
		stream << ", " << *_nodes.at(i);
	}
}

} // namespace ast
