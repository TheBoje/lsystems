#pragma once

#include "ast/tree/node.h"
#include "ast/tree/arg_list.h"
#include "ast/tree/identifier.h"

namespace ast {

class symbol : public node {
public:
	symbol(node* id, node_list* args = nullptr);
	void print(std::ostream& stream) const override;

public:
	identifier* _identifier = nullptr;
	arg_list* _args = nullptr;
};

} // namespace ast
