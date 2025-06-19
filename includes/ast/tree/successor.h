#pragma once

#include "ast/tree/node.h"
#include "node_list.h"

namespace ast {

class successor : public node {
public:
	successor(node_list* symbols);
	void print(std::ostream& stream) const override;

public:
	node_list* _symbols = nullptr;
};

} // namespace ast
