#include "ast/tree/definition.h"

namespace ast {

definition::definition(node* id, node* expression)
	: _identifier(static_cast<identifier*>(id))
	, _expression(static_cast<ast::abstract_expression*>(expression)) { }

definition::definition(definition&& other)
	: node(std::move(other))
	, _identifier(other._identifier)
	, _expression(other._expression) {
	other._identifier = nullptr;
	other._expression = nullptr;
}

definition::definition(const definition& other)
	: node(other) {
	_identifier = new ast::identifier(*other._identifier);
	_expression = other._expression->clone();
}

definition::~definition() {
	delete _identifier;
	delete _expression;
	_identifier = nullptr;
	_expression = nullptr;
}

definition& definition::operator=(definition&& other) {
	if (this != &other) {
		node::operator=(std::move(other));

		delete _identifier;
		delete _expression;

		_identifier = other._identifier;
		_expression = other._expression;

		other._identifier = nullptr;
		other._expression = nullptr;
	}
	return *this;
}

definition& definition::operator=(const definition& other) {
	if (this != &other) {
		node::operator=(other);
		delete _identifier;
		delete _expression;

		_identifier = other._identifier;
		_expression = other._expression;
	}
	return *this;
}

definition* definition::clone() const {
	return new ast::definition(*this);
}

void definition::print(std::ostream& stream) const {
	stream << *_identifier << " --> " << *_expression;
}

} // namespace ast
