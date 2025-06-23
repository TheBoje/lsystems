#pragma once

#include "ast/tree/node.h"

#include <vector>

namespace ast {

class node_list {
public:
	node_list(std::vector<node*> nodes = {});
	node_list(node_list&& other);
	node_list(const node_list& other);
	virtual ~node_list();
	virtual node_list& operator=(node_list&& rhs);
	virtual node_list& operator=(const node_list& rhs);

	virtual void print(std::ostream& stream) const;
	friend std::ostream& operator<<(std::ostream& os, const node_list& node);

	void push_back(node* n);

public:
	std::vector<node*> _nodes;
};

} // namespace ast
