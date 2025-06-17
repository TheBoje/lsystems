#pragma once

#include "ast/tree/node.h"

#include <ostream>
#include <string>

namespace ast {

class context : public node {
public:
	context(const std::string& symbols);

	void print(std::ostream& stream) const override;
	bool operator==(const context& other) const;
	bool operator!=(const context& other) const;

public:
	std::string context_symbols;
};

} // namespace ast
