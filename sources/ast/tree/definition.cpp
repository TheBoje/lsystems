#include "ast/tree/definition.h"

namespace ast {

definition::definition(node* id, node* expression)
	: _identifier(static_cast<identifier*>(id))
	, _expression(static_cast<ast::expression*>(expression)) { }

void definition::print(std::ostream& stream) const {
	stream << *_identifier << " --> " << *_expression;
}

} // namespace ast
