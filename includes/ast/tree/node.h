#pragma once

#include <ostream>

namespace ast {

class node {
public:
	virtual ~node() { }
	virtual void print(std::ostream& stream) const = 0;

	friend std::ostream& operator<<(std::ostream& os, const node& node);
};

} // namespace ast
