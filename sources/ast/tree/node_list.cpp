#include "ast/tree/node_list.h"

namespace ast {

node_list::node_list(std::vector<node*> nodes)
	: _nodes(nodes) { }

node_list::node_list(node_list&& other)
	: _nodes(other._nodes) {
	other._nodes = {};
}

node_list::node_list(const node_list& other) {
	_nodes = {};
	for (auto node : other._nodes) {
		_nodes.push_back(node->clone());
	}
}

node_list::~node_list() {
	for (auto node : _nodes) {
		delete node;
	}
	_nodes = {};
}

node_list& node_list::operator=(node_list&& rhs) {
	if (this != &rhs) {
		for (auto node : _nodes) {
			delete node;
		}
		_nodes = rhs._nodes;

		for (auto node : rhs._nodes) {
			delete node;
		}
		rhs._nodes = {};
	}

	return *this;
}

node_list& node_list::operator=(const node_list& rhs) {
	if (this != &rhs) {
		for (auto node : _nodes) {
			delete node;
		}
		_nodes = rhs._nodes;
	}

	return *this;
}

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
