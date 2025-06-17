#pragma once

#include "ast/tree/node.h"

namespace ast {

class context;
class symbol;
class replacement;

class production : public node {
public:
	production(context* left, symbol* symbol, context* right, replacement* replacement);

	void print(std::ostream& stream) const override;
	bool operator==(const production& other) const;
	bool operator!=(const production& other) const;

public:
	context* left_context = nullptr;
	context* right_context = nullptr;
	symbol* symbol_node = nullptr;
	replacement* replacement_node = nullptr;
};

} // namespace ast
