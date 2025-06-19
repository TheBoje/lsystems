#pragma once

#include "ast/tree/node.h"
#include "ast/tree/node_list.h"

namespace ast {

class productions : public node {
public:
	productions(node_list* production_list);
	void print(std::ostream& stream) const override;

public:
	node_list* _production_list = nullptr;
};
} // namespace ast
