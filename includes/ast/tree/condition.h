#pragma once

#include "ast/tree/node.h"
#include "ast/tree/expression.h"
#include "ast/tree/comparator.h"

namespace ast {

class condition : public node {
public:
	condition(node* lhs, node* rhs, node* comp);
	void print(std::ostream& stream) const override;

public:
	expression* _lhs = nullptr;
	expression* _rhs = nullptr;
	comparator* _comparator = nullptr;
};
} // namespace ast
