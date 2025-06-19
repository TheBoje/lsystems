#include "ast/tree/predecessor.h"

namespace ast {

predecessor::predecessor(node_list* symbols, node_list* left_ctx, node_list* right_ctx)
	: _symbols(symbols)
	, _left_context(left_ctx)
	, _right_context(right_ctx) { }

void predecessor::print(std::ostream& stream) const {
	if (_left_context) {
		stream << *_left_context << " < ";
	}

	stream << *_symbols;

	if (_right_context) {
		stream << " > " << *_right_context;
	}
}

} // namespace ast
