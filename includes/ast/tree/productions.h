#pragma once

#include "ast/tree/node.h"
#include "ast/tree/production_list.h"

namespace ast {

class productions : public node {
public:
	productions(node_list* production_lst);
	productions(productions&& other);
	productions(const productions& other);
	virtual ~productions() override;
	virtual productions& operator=(productions&& rhs);
	virtual productions& operator=(const productions& rhs);
	virtual productions* clone() const override;

	void print(std::ostream& stream) const override;

	std::pair<size_t, size_t> get_context_maxes() const;

public:
	production_list* _production_list = nullptr;
};
} // namespace ast
