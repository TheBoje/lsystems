#pragma once

#include "ast/tree/node.h"

#include <string>

namespace ast {

class identifier : public node {
public:
	identifier(const std::string& value);
	void print(std::ostream& stream) const override;

public:
	std::string _value;
};

} // namespace ast
