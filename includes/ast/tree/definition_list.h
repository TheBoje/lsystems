#pragma once

#include "ast/tree/node_list.h"

namespace ast {

class definition_list : public node_list {
public:
	definition_list(std::vector<node*> nodes = {});
	definition_list(definition_list&& other);
	definition_list(const definition_list& other);

	void print(std::ostream& stream) const override;
};
} // namespace ast
