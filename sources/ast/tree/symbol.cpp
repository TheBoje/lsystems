#include "ast/tree/symbol.h"

namespace ast {

symbol::symbol(node* id, node_list* args)
	: _identifier(static_cast<ast::identifier*>(id))
	, _args(static_cast<ast::arg_list*>(args)) { }

void symbol::print(std::ostream& stream) const {
	stream << *_identifier;
	if (_args) {
		stream << "(" << *_args << ")";
	}
}

} // namespace ast
