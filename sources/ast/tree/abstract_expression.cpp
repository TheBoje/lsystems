#include "ast/tree/abstract_expression.h"
#include "macros.h"

namespace ast {

abstract_expression& abstract_expression::operator=(const abstract_expression& other) {
	UNUSED(other);
	return *this;
}

abstract_expression& abstract_expression::operator=(abstract_expression&& other) {
	UNUSED(other);
	return *this;
}

} // namespace ast
