#pragma once

#include "ast/tree/node.h"
#include "ast/tree/abstract_expression.h"
#include "ast/tree/identifier.h"

namespace ast {

class definition : public node {
public:
	definition(node* id, node* expression);
	definition(definition&& other);
	definition(const definition& other);
	virtual ~definition() override;
	virtual definition& operator=(definition&& rhs);
	virtual definition& operator=(const definition& rhs);
	virtual definition* clone() const override;

	void print(std::ostream& stream) const override;

public:
	identifier* _identifier = nullptr;
	abstract_expression* _expression = nullptr;
};
} // namespace ast
