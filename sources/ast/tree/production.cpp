#include "ast/tree/production.h"

namespace ast {

production::production(node* predecessor, node* successor, node* condition)
	: _predecessor(static_cast<ast::predecessor*>(predecessor))
	, _condition(static_cast<ast::condition*>(condition))
	, _successor(static_cast<ast::successor*>(successor)) { }

void production::print(std::ostream& stream) const {
	stream << *_predecessor;

	if (_condition) {
		stream << " : " << *_condition;
	}

	stream << " --> " << *_successor;
}

} // namespace ast
