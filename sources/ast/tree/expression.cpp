#include "ast/tree/expression.h"
#include "numerical.h"
#include <cassert>
#include <cmath>

namespace ast {

expression::expression(ast::node* lhs, ast::node* rhs, ast::operator_type op)
	: _lhs(static_cast<ast::expression*>(lhs))
	, _rhs(static_cast<ast::expression*>(rhs))
	, _operator(op) { }

expression::expression(expression&& other)
	: abstract_expression(std::move(other))
	, _lhs(other._lhs)
	, _rhs(other._rhs)
	, _operator(other._operator) {
	other._lhs = nullptr;
	other._rhs = nullptr;
	other._operator = ast::operator_type();
}

expression::expression(const expression& other)
	: abstract_expression(other) {
	_lhs = other._lhs->clone();
	_rhs = other._rhs->clone();
	_operator = ast::operator_type(other._operator);
}

expression::~expression() {
	delete _lhs;
	delete _rhs;
}

expression& expression::operator=(expression&& rhs) {
	if (this != &rhs) {
		node::operator=(std::move(rhs));

		delete _lhs;
		delete _rhs;

		_lhs = rhs._lhs;
		_rhs = rhs._rhs;
		_operator = rhs._operator;

		rhs._lhs = nullptr;
		rhs._rhs = nullptr;
		rhs._operator = ast::operator_type();
	}

	return *this;
}

expression& expression::operator=(const expression& rhs) {
	if (this != &rhs) {
		node::operator=(rhs);

		delete _lhs;
		delete _rhs;

		_lhs = rhs._lhs;
		_rhs = rhs._rhs;
		_operator = rhs._operator;
	}

	return *this;
}

expression* expression::clone() const {
	return new ast::expression(*this);
}

abstract_expression* expression::evaluate(std::vector<definition*> local_definitions, ast::definitions* global_definitions) const {
	auto lhs_evaluated = _lhs->evaluate(local_definitions, global_definitions);
	auto rhs_evaluated = _rhs->evaluate(local_definitions, global_definitions);

	double lhs_value = 0.0f;
	double rhs_value = 0.0f;

	if (dynamic_cast<ast::numerical<int>*>(lhs_evaluated)) {
		lhs_value = (double)dynamic_cast<ast::numerical<int>*>(lhs_evaluated)->_value;
	} else {
		lhs_value = dynamic_cast<ast::numerical<double>*>(lhs_evaluated)->_value;
	}

	if (dynamic_cast<ast::numerical<int>*>(rhs_evaluated)) {
		rhs_value = (double)dynamic_cast<ast::numerical<int>*>(rhs_evaluated)->_value;
	} else {
		rhs_value = dynamic_cast<ast::numerical<double>*>(rhs_evaluated)->_value;
	}

	double result = 0.0f;

	switch (_operator) {
		case PLUS: {
			result = lhs_value + rhs_value;
			break;
		}
		case MINUS: {
			result = lhs_value - rhs_value;
			break;
		}

		case MULT: {
			result = lhs_value * rhs_value;
			break;
		}

		case DIVIDE: {
			assert(rhs_value != 0);
			result = lhs_value / rhs_value;
			break;
		}

		case EXPONENT: {
			result = std::pow(lhs_value, rhs_value);
			break;
		}
	}

	delete lhs_evaluated;
	delete rhs_evaluated;

	return new ast::numerical<double>(result);
}

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
