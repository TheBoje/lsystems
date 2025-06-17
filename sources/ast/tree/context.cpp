#include "ast/tree/context.h"

namespace ast {

context::context(const std::string& symbols)
	: context_symbols(symbols) { }

void context::print(std::ostream& stream) const {
	stream << context_symbols;
}

bool context::operator==(const context& other) const {
	return context_symbols == other.context_symbols;
}

bool context::operator!=(const context& other) const {
	return !(*this == other);
}

} // namespace ast
