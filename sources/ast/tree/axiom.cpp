#include "ast/tree/axiom.h"
#include <utility>

namespace ast {

axiom::axiom(node_list* symbols)
	: _symbols(symbols) { }

axiom::axiom(axiom&& other)
	: node(std::move(other))
	, _symbols(other._symbols) {
	other._symbols = nullptr;
}

axiom::axiom(const axiom& other)
	: node(other) {
	if (other._symbols) {
		_symbols = new node_list(*other._symbols); // deep copy
	}
}

axiom::~axiom() {
	delete _symbols;
	_symbols = nullptr;
}

axiom& axiom::operator=(axiom&& other) {
	if (this != &other) {
		node::operator=(std::move(other));
		delete _symbols;
		_symbols = nullptr;
		if (other._symbols) {
			_symbols = new ast::node_list(*other._symbols);
			other._symbols = nullptr;
		}
	}
	return *this;
}

axiom& axiom::operator=(const axiom& other) {
	if (this != &other) {
		node::operator=(other);
		delete _symbols;
		_symbols = nullptr;
		if (other._symbols) {
			_symbols = new ast::node_list(*other._symbols);
		}
	}
	return *this;
}

axiom* axiom::clone() const {
	return new ast::axiom(*this);
}

void axiom::print(std::ostream& stream) const {
	stream << "axiom: " << *_symbols << std::endl;
}

} // namespace ast
