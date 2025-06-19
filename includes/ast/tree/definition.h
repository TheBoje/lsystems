#pragma once

#include "ast/tree/node.h"
#include "ast/tree/identifier.h"
#include "ast/tree/expression.h"

namespace ast {

class definition : public node {
public:
	definition(node* id, node* expression);
	void print(std::ostream& stream) const override;

public:
	identifier* _identifier = nullptr;
	expression* _expression = nullptr;
};
} // namespace ast
