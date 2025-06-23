#pragma once

#include "ast/tree/node.h"
#include "ast/tree/node_list.h"

namespace ast {
class axiom : public node {
public:
	axiom(node_list* symbols);
	axiom(axiom&& other);
	axiom(const axiom& other);
	virtual ~axiom() override;
	virtual axiom& operator=(axiom&& rhs);
	virtual axiom& operator=(const axiom& rhs);
	virtual axiom* clone() const override;

	void print(std::ostream& stream) const override;

public:
	node_list* _symbols = nullptr;
};
} // namespace ast
