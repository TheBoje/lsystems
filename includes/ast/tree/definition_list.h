#pragma once

#include "ast/tree/node_list.h"

namespace ast {

class definition_list : public node_list {
public:
	definition_list(std::vector<node*> nodes = {});
	void print(std::ostream& stream) const override;
};
} // namespace ast
