#pragma once

#include "ast/tree/node_list.h"

namespace ast {

class production_list : public node_list {
public:
	production_list(std::vector<node*> nodes = {});
	production_list(production_list&& other);
	production_list(const production_list& other);

	void print(std::ostream& stream) const override;
};

} // namespace ast
