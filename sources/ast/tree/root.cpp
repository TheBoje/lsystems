#include "ast/tree/root.h"
#include "definitions.h"

namespace ast {

root::root(node* axiom, node* definitions, node* productions)
	: _axiom(static_cast<ast::axiom*>(axiom))
	, _definitions(static_cast<ast::definitions*>(definitions))
	, _productions(static_cast<ast::productions*>(productions)) { }

void root::print(std::ostream& stream) const {
	stream << *_axiom << *_definitions << *_productions << std::endl;
}

} // namespace ast
