#include "ast/tree/productions.h"

namespace ast {

productions::productions(node_list* production_list)
	: _production_list(production_list) { }

void productions::print(std::ostream& stream) const {
	stream << "productions:" << *_production_list;
}

} // namespace ast
