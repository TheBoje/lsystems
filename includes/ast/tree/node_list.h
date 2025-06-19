#pragma once

#include "ast/tree/node.h"

#include <vector>

namespace ast {

class node_list {
public:
	node_list(std::vector<node*> nodes = {});

	virtual void print(std::ostream& stream) const;
	friend std::ostream& operator<<(std::ostream& os, const node_list& node);

	void push_back(node* n);

public:
	std::vector<node*> _nodes;
};

} // namespace ast
