#include "ast/tree/node_list.h"

namespace ast {

node_list::node_list(std::vector<node*> nodes)
	: _nodes(nodes) { }

void node_list::print(std::ostream& stream) const {
	for (auto node : _nodes) {
		stream << *node << " ";
	}
}

std::ostream& operator<<(std::ostream& os, const node_list& node) {
	node.print(os);
	return os;
}

void node_list::push_back(node* n) {
	_nodes.push_back(n);
}

} // namespace ast
