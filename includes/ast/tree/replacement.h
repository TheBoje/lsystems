#pragma once

#include "ast/tree/node.h"

namespace ast {

class symbol;

class replacement : public node {
public:
	replacement(symbol* symbols);
	void print(std::ostream& stream) const override;
	bool operator==(const replacement& other) const;
	bool operator!=(const replacement& other) const;

public:
	symbol* result_symbol;
};
} // namespace ast
