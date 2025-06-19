#include "ast/tree/identifier.h"

namespace ast {

identifier::identifier(const std::string& value)
	: _value(value) { }

void identifier::print(std::ostream& stream) const {
	stream << _value;
}

} // namespace ast
