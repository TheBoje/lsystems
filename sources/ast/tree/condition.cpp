#include "ast/tree/condition.h"
#include "comparator.h"

namespace ast {

condition::condition(node* lhs, node* rhs, node* comp)
	: _lhs(static_cast<expression*>(lhs))
	, _rhs(static_cast<expression*>(rhs))
	, _comparator(static_cast<comparator*>(comp)) { }

void condition::print(std::ostream& stream) const {
	stream << *_lhs << " " << *_comparator << " " << *_rhs;
}

} // namespace ast
