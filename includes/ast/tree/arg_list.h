#pragma once

#include "ast/tree/node_list.h"

namespace ast {

class arg_list : public node_list {
public:
	arg_list(std::vector<node*> nodes = {});
	void print(std::ostream& stream) const override;
};
} // namespace ast
