#pragma once

#include "ast/tree/node.h"
#include "ast/tree/definition_list.h"

namespace ast {
class definitions : public node {
public:
	definitions(node_list* definition_list);
	definitions(definitions&& other);
	definitions(const definitions& other);
	virtual ~definitions() override;
	virtual definitions& operator=(definitions&& rhs);
	virtual definitions& operator=(const definitions& rhs);
	virtual definitions* clone() const override;

	void print(std::ostream& stream) const override;

public:
	definition_list* _definition_list = nullptr;
};
} // namespace ast
