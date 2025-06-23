#include "ast/tree/successor.h"

namespace ast {

successor::successor(node_list* symbols)
	: _symbols(symbols) { }

successor::successor(successor&& other)
	: node(std::move(other))
	, _symbols(other._symbols) {
	other._symbols = nullptr;
}

successor::successor(const successor& other)
	: node(other) {
	_symbols = new ast::node_list(*other._symbols);
}

successor::~successor() {
	delete _symbols;
}

successor& successor::operator=(successor&& rhs) {
	if (this != &rhs) {
		node::operator=(std::move(rhs));

		delete _symbols;

		_symbols = rhs._symbols;

		rhs._symbols = nullptr;
	}

	return *this;
}

successor& successor::operator=(const successor& rhs) {
	if (this != &rhs) {
		node::operator=(rhs);

		delete _symbols;

		_symbols = rhs._symbols;
	}

	return *this;
}

successor* successor::clone() const {
	return new ast::successor(*this);
}

void successor::print(std::ostream& stream) const {
	stream << *_symbols;
}

} // namespace ast
