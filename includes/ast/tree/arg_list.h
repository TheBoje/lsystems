#pragma once

#include "ast/tree/node_list.h"

namespace ast {

class arg_list : public node_list {
public:
	arg_list(std::vector<node*> nodes = {});
	arg_list(arg_list&& other);
	arg_list(const arg_list& other);

	void print(std::ostream& stream) const override;
};
} // namespace ast
