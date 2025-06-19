#include "ast/tree/expression.h"

namespace ast {

expression::expression(ast::node* lhs, ast::node* rhs, ast::operator_type op)
	: _lhs(static_cast<ast::expression*>(lhs))
	, _rhs(static_cast<ast::expression*>(rhs))
	, _operator(op) { }

void expression::print(std::ostream& stream) const {
	std::string operator_type_print;
	switch (_operator) {
		case PLUS: {
			operator_type_print = "+";
			break;
		}
		case MINUS: {
			operator_type_print = "-";
			break;
		}
		case MULT: {
			operator_type_print = "*";
			break;
		}
		case DIVIDE: {
			operator_type_print = "/";
			break;
		}
		case EXPONENT: {
			operator_type_print = "^";
			break;
		}
	}

	stream << *_lhs << " " << operator_type_print << " " << *_rhs;
}

} // namespace ast
