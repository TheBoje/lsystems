#pragma once

#include "ast/tree/node.h"
#include "ast/tree/abstract_expression.h"
#include "ast/tree/comparator.h"

namespace ast {

class condition : public node {
public:
	condition(node* lhs, node* rhs, node* comp);
	condition(condition&& other);
	condition(const condition& other);
	virtual ~condition() override;
	virtual condition& operator=(condition&& rhs);
	virtual condition& operator=(const condition& rhs);
	virtual condition* clone() const override;
	bool evaluate(std::vector<definition*> local_definitions, ast::definitions* global_definitions) const;

	void print(std::ostream& stream) const override;

public:
	abstract_expression* _lhs = nullptr;
	abstract_expression* _rhs = nullptr;
	comparator* _comparator = nullptr;
};
} // namespace ast
