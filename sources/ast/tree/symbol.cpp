#include "ast/tree/symbol.h"

namespace ast {

symbol::symbol(node* id, node_list* args)
	: _identifier(static_cast<ast::identifier*>(id))
	, _args(static_cast<ast::arg_list*>(args)) { }

symbol::symbol(symbol&& other)
	: node(std::move(other))
	, _identifier(other._identifier)
	, _args(other._args) {
	other._identifier = nullptr;
	other._args = nullptr;
}

symbol::symbol(const symbol& other)
	: node(other) {
	_identifier = new ast::identifier(*other._identifier);
	if (other._args) {
		_args = new ast::arg_list(*other._args);
	}
}

symbol::~symbol() {
	delete _identifier;
	delete _args;
}

symbol& symbol::operator=(symbol&& rhs) {
	if (this != &rhs) {
		node::operator=(std::move(rhs));

		delete _identifier;
		delete _args;

		_identifier = rhs._identifier;
		_args = rhs._args;

		rhs._identifier = nullptr;
		rhs._args = nullptr;
	}

	return *this;
}

symbol& symbol::operator=(const symbol& rhs) {
	if (this != &rhs) {
		node::operator=(rhs);

		delete _identifier;
		delete _args;

		_identifier = rhs._identifier;
		_args = rhs._args;
	}

	return *this;
}

symbol* symbol::clone() const {
	return new ast::symbol(*this);
}

void symbol::print(std::ostream& stream) const {
	stream << *_identifier;
	if (_args) {
		stream << "(" << *_args << ")";
	}
}

} // namespace ast
