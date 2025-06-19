#include "ast/tree/definitions.h"

namespace ast {

definitions::definitions(node_list* definition_list)
	: _definition_list(static_cast<ast::definition_list*>(definition_list)) { }

void definitions::print(std::ostream& stream) const {
	stream << "definitions: " << *_definition_list << std::endl;
}

} // namespace ast
