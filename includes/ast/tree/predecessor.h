#pragma once

#include "ast/tree/node.h"
#include "ast/tree/node_list.h"
#include "ast/tree/symbol.h"

namespace ast {

class predecessor : public node {
public:
	predecessor(ast::node* symbol, node_list* left_ctx = nullptr, node_list* right_ctx = nullptr);
	predecessor(predecessor&& other);
	predecessor(const predecessor& other);
	virtual ~predecessor() override;
	virtual predecessor& operator=(predecessor&& rhs);
	virtual predecessor& operator=(const predecessor& rhs);
	virtual predecessor* clone() const override;

	void print(std::ostream& stream) const override;

	bool matches_symbol(ast::symbol* symbol, ast::node_list* left_context, ast::node_list* right_context) const;
	std::vector<ast::definition*> get_local_definitions(ast::symbol* symbol, ast::node_list* left_context, ast::node_list* right_context) const;

private:
	bool matches_context_left(ast::node_list* context) const;
	bool matches_context_right(ast::node_list* context) const;

public:
	symbol* _symbol = nullptr;
	node_list* _left_context = nullptr;
	node_list* _right_context = nullptr;
};

} // namespace ast
