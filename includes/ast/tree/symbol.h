#pragma once

#include "ast/tree/node.h"
#include "ast/tree/arg_list.h"
#include "ast/tree/identifier.h"

namespace ast {

class symbol : public node {
public:
	symbol(node* id, node_list* args = nullptr);
	symbol(symbol&& other);
	symbol(const symbol& other);
	virtual ~symbol() override;
	virtual symbol& operator=(symbol&& rhs);
	virtual symbol& operator=(const symbol& rhs);
	virtual symbol* clone() const override;

	void print(std::ostream& stream) const override;

public:
	identifier* _identifier = nullptr;
	arg_list* _args = nullptr;
};

} // namespace ast
