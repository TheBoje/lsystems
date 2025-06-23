#pragma once

#include "ast/tree/abstract_expression.h"

#include <string>

namespace ast {

class identifier : public abstract_expression {
public:
	identifier(const std::string& value);
	virtual identifier* clone() const override;
	virtual bool operator==(const identifier& rhs) const;
	abstract_expression* evaluate(std::vector<definition*> local_definitions, ast::definitions* global_definitions) const override;

	void print(std::ostream& stream) const override;

public:
	std::string _value;
};

} // namespace ast
