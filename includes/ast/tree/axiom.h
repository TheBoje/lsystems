#pragma once

#include "ast/tree/node.h"
#include "ast/tree/node_list.h"

namespace ast {
class axiom : public node {
public:
	axiom(node_list* symbols);
	void print(std::ostream& stream) const override;

public:
	node_list* _symbols = nullptr;
};
} // namespace ast
