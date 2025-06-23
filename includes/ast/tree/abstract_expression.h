#pragma once

#include "ast/tree/node.h"

#include <vector>

namespace ast {

class definition;
class definitions;

class abstract_expression : public node {
public:
	abstract_expression() = default;
	abstract_expression(abstract_expression&& other) = default;
	abstract_expression(const abstract_expression& other) = default;
	virtual ~abstract_expression() override = default;
	virtual abstract_expression& operator=(abstract_expression&& rhs);
	virtual abstract_expression& operator=(const abstract_expression& rhs);
	virtual abstract_expression* clone() const override = 0;

	virtual abstract_expression* evaluate(std::vector<definition*> local_definitions, ast::definitions* global_definitions) const = 0;
};

} // namespace ast
