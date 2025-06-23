#pragma once

#include "ast/tree/node.h"
#include "node_list.h"

namespace ast {

class successor : public node {
public:
	successor(node_list* symbols);
	successor(successor&& other);
	successor(const successor& other);
	virtual ~successor() override;
	virtual successor& operator=(successor&& rhs);
	virtual successor& operator=(const successor& rhs);
	virtual successor* clone() const override;

	void print(std::ostream& stream) const override;

public:
	node_list* _symbols = nullptr;
};

} // namespace ast
