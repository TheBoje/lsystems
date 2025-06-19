#pragma once

#include "ast/tree/node.h"
#include "ast/tree/node_list.h"

namespace ast {

class predecessor : public node {
public:
	predecessor(node_list* symbols, node_list* left_ctx = nullptr, node_list* right_ctx = nullptr);
	void print(std::ostream& stream) const override;

public:
	node_list* _symbols = nullptr;
	node_list* _left_context = nullptr;
	node_list* _right_context = nullptr;
};

} // namespace ast
