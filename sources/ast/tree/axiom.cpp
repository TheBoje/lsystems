#include "ast/tree/axiom.h"

namespace ast {

axiom::axiom(node_list* symbols)
	: _symbols(symbols) { }

void axiom::print(std::ostream& stream) const {
	stream << "axiom: " << *_symbols << std::endl;
}

} // namespace ast
