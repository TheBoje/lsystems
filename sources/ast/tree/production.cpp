#include "ast/tree/production.h"

#include "ast/tree/context.h"
#include "ast/tree/symbol.h"
#include "ast/tree/replacement.h"

#include <ostream>

namespace ast {

production::production(context* left, symbol* symbol, context* right, replacement* replacement)
	: left_context(left)
	, right_context(right)
	, symbol_node(symbol)
	, replacement_node(replacement) { }

void production::print(std::ostream& stream) const {
	if (left_context)
		stream << *left_context;
	else
		stream << "*";

	stream << " < " << *symbol_node << " > ";

	if (right_context)
		stream << *right_context;
	else
		stream << "*";

	stream << " --> " << *replacement_node;
}
bool production::operator==(const production& other) const {
	return left_context == other.left_context && right_context == other.right_context && symbol_node == other.symbol_node && replacement_node == other.replacement_node;
}
bool production::operator!=(const production& other) const {
	return !(*this == other);
}
} // namespace ast
