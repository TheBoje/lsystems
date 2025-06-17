#include "ast/tree/node.h"

namespace ast {

std::ostream& operator<<(std::ostream& os, const node& node) {
	node.print(os);
	return os;
}

} // namespace ast
