#pragma once

#include "ast/tree/abstract_expression.h"

namespace ast {

enum operator_type { PLUS, MINUS, MULT, DIVIDE, EXPONENT };

class expression : public abstract_expression {
public:
	expression(ast::node* lhs, ast::node* rhs, ast::operator_type op);
	expression(expression&& other);
	expression(const expression& other);
	virtual ~expression() override;
	virtual expression& operator=(expression&& rhs);
	virtual expression& operator=(const expression& rhs);
	virtual expression* clone() const override;
	abstract_expression* evaluate(std::vector<definition*> local_definitions, ast::definitions* global_definitions) const override;

	void print(std::ostream& stream) const override;

public:
	ast::expression* _lhs = nullptr;
	ast::expression* _rhs = nullptr;
	ast::operator_type _operator;
};

} // namespace ast
