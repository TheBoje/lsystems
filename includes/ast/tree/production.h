#pragma once

#include "ast/tree/node.h"
#include "ast/tree/predecessor.h"
#include "ast/tree/successor.h"
#include "ast/tree/condition.h"

namespace ast {

class production : public node {
public:
	production(node* predecessor, node* successor, node* condition = nullptr);
	void print(std::ostream& stream) const override;

public:
	predecessor* _predecessor = nullptr;
	condition* _condition = nullptr;
	successor* _successor = nullptr;
};

} // namespace ast
