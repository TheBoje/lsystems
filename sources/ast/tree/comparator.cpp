#include "ast/tree/comparator.h"
#include "macros.h"

namespace ast {

comparator::comparator(comparator_type type)
	: _type(type) { }

void comparator::print(std::ostream& stream) const {
	switch (_type) {
		case ast::comparator_type::LT: {
			stream << "<";
			break;
		}
		case LTE: {
			stream << "<=";
			break;
		}

		case GT: {
			stream << ">";
			break;
		}

		case GTE: {
			stream << ">=";
			break;
		}

		case EQ: {
			stream << "==";
			break;
		}

		case NEQ: {
			stream << "!=";
			break;
		}
	}
}

} // namespace ast
