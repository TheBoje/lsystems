#pragma once

#include "ast/tree/node.h"

namespace ast {

class symbol : public node {
public:
	symbol(const std::string& symbol);

	void print(std::ostream& stream) const override;
	bool operator==(const symbol& other) const;
	bool operator!=(const symbol& other) const;

public:
	std::string symbols;
};

} // namespace ast
