#include "ast/tree/replacement.h"

#include "ast/tree/symbol.h"

#include <ostream>

namespace ast {

replacement::replacement(symbol* symbols)
	: result_symbol(symbols) { }

void replacement::print(std::ostream& stream) const {
	stream << *result_symbol;
}

bool replacement::operator==(const replacement& other) const {
	return result_symbol == other.result_symbol;
}
bool replacement::operator!=(const replacement& other) const {
	return !(*this == other);
}

} // namespace ast
