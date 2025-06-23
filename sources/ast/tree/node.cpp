#include "ast/tree/node.h"
#include "macros.h"

namespace ast {

node::~node() = default;

std::ostream& operator<<(std::ostream& os, const node& node) {
	node.print(os);
	return os;
}

node& node::operator=(const node& other) {
	UNUSED(other);
	return *this;
}

node& node::operator=(node&& other) {
	UNUSED(other);
	return *this;
}

bool node::operator!=(const node& rhs) const {
	return !(*this == rhs);
}

} // namespace ast
