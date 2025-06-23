#pragma once

#include "ast/tree/node.h"

namespace ast {
enum comparator_type { LT, LTE, GT, GTE, EQ, NEQ };

class comparator : public node {
public:
	comparator(comparator_type type);
	virtual comparator* clone() const override;

	void print(std::ostream& stream) const override;

public:
	const comparator_type _type;
};
} // namespace ast
