#include "ast/tree/condition.h"
#include "ast/tree/abstract_expression.h"
#include "ast/tree/comparator.h"
#include "ast/tree/numerical.h"

#include <utility>

namespace ast {

condition::condition(node* lhs, node* rhs, node* comp)
	: _lhs(static_cast<abstract_expression*>(lhs))
	, _rhs(static_cast<abstract_expression*>(rhs))
	, _comparator(static_cast<comparator*>(comp)) { }

condition::condition(condition&& other)
	: node(std::move(other))
	, _lhs(other._lhs)
	, _rhs(other._rhs)
	, _comparator(other._comparator) {
	other._lhs = nullptr;
	other._rhs = nullptr;
	other._comparator = nullptr;
}

condition::condition(const condition& other)
	: node(other) {
	_lhs = other._lhs->clone();
	_rhs = other._rhs->clone();
	_comparator = new ast::comparator(*other._comparator);
}

condition::~condition() {
	delete _lhs;
	delete _rhs;
	delete _comparator;
}

condition& condition::operator=(condition&& rhs) {
	if (this != &rhs) {
		node::operator=(std::move(rhs));

		delete _lhs;
		delete _rhs;
		delete _comparator;

		_lhs = rhs._lhs;
		_rhs = rhs._rhs;
		_comparator = rhs._comparator;

		rhs._lhs = nullptr;
		rhs._rhs = nullptr;
		rhs._comparator = nullptr;
	}

	return *this;
}

condition& condition::operator=(const condition& rhs) {
	if (this != &rhs) {
		node::operator=(rhs);

		delete _lhs;
		delete _rhs;
		delete _comparator;

		_lhs = rhs._lhs;
		_rhs = rhs._rhs;
		_comparator = rhs._comparator;
	}

	return *this;
}

condition* condition::clone() const {
	return new ast::condition(*this);
}

bool condition::evaluate(std::vector<definition*> local_definitions, ast::definitions* global_definitions) const {
	auto lhs_evaluated = _lhs->evaluate(local_definitions, global_definitions);
	auto rhs_evaluated = _rhs->evaluate(local_definitions, global_definitions);

	double lhs_value = 0.0f;
	double rhs_value = 0.0f;

	if (dynamic_cast<ast::numerical<int>*>(lhs_evaluated)) {
		lhs_value = (float)dynamic_cast<ast::numerical<int>*>(lhs_evaluated)->_value;
	} else {
		lhs_value = dynamic_cast<ast::numerical<double>*>(lhs_evaluated)->_value;
	}

	if (dynamic_cast<ast::numerical<int>*>(rhs_evaluated)) {
		rhs_value = (double)dynamic_cast<ast::numerical<int>*>(rhs_evaluated)->_value;
	} else {
		rhs_value = dynamic_cast<ast::numerical<double>*>(rhs_evaluated)->_value;
	}

	switch (_comparator->_type) {
		case LT: {
			return lhs_value < rhs_value;
		}
		case LTE: {
			return lhs_value <= rhs_value;
		}
		case GT: {
			return lhs_value > rhs_value;
		}
		case GTE: {
			return lhs_value >= rhs_value;
		}
		case EQ: {
			return lhs_value == rhs_value;
		}
		case NEQ: {
			return lhs_value != rhs_value;
		}
	}
}

void condition::print(std::ostream& stream) const {
	stream << *_lhs << " " << *_comparator << " " << *_rhs;
}

} // namespace ast
