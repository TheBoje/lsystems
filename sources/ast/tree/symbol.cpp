#include "ast/tree/symbol.h"

namespace ast {

symbol::symbol(const std::string& symbol)
	: symbols(symbol) { }

void symbol::print(std::ostream& stream) const {
	stream << symbols;
}

bool symbol::operator==(const symbol& other) const {
	return symbols == other.symbols;
}
bool symbol::operator!=(const symbol& other) const {
	return !(*this == other);
}

} // namespace ast
