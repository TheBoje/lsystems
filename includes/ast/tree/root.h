#pragma once

#include "ast/tree/node.h"
#include "ast/tree/axiom.h"
#include "ast/tree/definitions.h"
#include "ast/tree/productions.h"

namespace ast {
class root : public node {
public:
	root(node* axiom, node* definitions, node* productions);
	void print(std::ostream& stream) const override;

public:
	axiom* _axiom = nullptr;
	definitions* _definitions = nullptr;
	productions* _productions = nullptr;
};
} // namespace ast
