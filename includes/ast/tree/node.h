#pragma once

#include <ostream>

namespace ast {

class node {
public:
	node() = default;
	node(node&&) = default;
	node(const node&) = default;
	virtual ~node();
	virtual node& operator=(node&&);
	virtual node& operator=(const node&);
	virtual node* clone() const = 0;

	virtual bool operator==(const node&) const {
		return true;
	};
	bool operator!=(const node& rhs) const;

	virtual void print(std::ostream& stream) const = 0;
	friend std::ostream& operator<<(std::ostream& os, const node& node);
};

} // namespace ast
