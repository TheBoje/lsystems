#include "ast/tree/successor.h"

namespace ast {

successor::successor(node_list* symbols)
	: _symbols(symbols) { }

void successor::print(std::ostream& stream) const {
	stream << *_symbols;
}

} // namespace ast
