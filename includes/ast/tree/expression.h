#pragma once

#include "ast/tree/node.h"

namespace ast {

enum operator_type { PLUS, MINUS, MULT, DIVIDE, EXPONENT };

class expression : public node {
public:
	expression(ast::node* lhs, ast::node* rhs, ast::operator_type op);
	void print(std::ostream& stream) const override;

public:
	ast::expression* _lhs = nullptr;
	ast::expression* _rhs = nullptr;
	ast::operator_type _operator;
};

} // namespace ast
