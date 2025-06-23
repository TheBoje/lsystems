#pragma once

#include "ast/tree/node.h"
#include "ast/tree/predecessor.h"
#include "ast/tree/successor.h"
#include "ast/tree/condition.h"
#include "ast/tree/symbol.h"
#include "ast/tree/definitions.h"

namespace ast {

class production : public node {
public:
	production(node* predecessor, node* successor, node* condition = nullptr);
	production(production&& other);
	production(const production& other);
	virtual ~production() override;
	virtual production& operator=(production&& rhs);
	virtual production& operator=(const production& rhs);
	virtual production* clone() const override;

	void print(std::ostream& stream) const override;

	bool matches_symbol(ast::symbol* symbol, ast::node_list* left_context, ast::node_list* right_context, ast::definitions* definitions) const;
	void add_evaluated_successor(ast::symbol* source_symbol, ast::node_list* result, ast::definitions* definitions) const;

private:
	bool satisfy_conditions(ast::symbol* symbol, ast::definitions* definitions) const;

public:
	predecessor* _predecessor = nullptr;
	condition* _condition = nullptr;
	successor* _successor = nullptr;
};

} // namespace ast
