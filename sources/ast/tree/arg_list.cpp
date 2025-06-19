#include "ast/tree/arg_list.h"

namespace ast {

arg_list::arg_list(std::vector<node*> nodes)
	: node_list(nodes) { }

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
