#pragma once

#include "ast/tree/abstract_expression.h"
#include "macros.h"

namespace ast {

template <typename T>
class numerical : public abstract_expression {
public:
	numerical(T value)
		: _value(value) { }

	virtual numerical<T>* clone() const override {
		return new ast::numerical<T>(*this);
	}

	void print(std::ostream& stream) const override {
		stream << _value;
	}

	abstract_expression* evaluate(std::vector<definition*> local_definitions, ast::definitions* global_definitions) const override {
		UNUSED(local_definitions);
		UNUSED(global_definitions);

		return clone();
	}

public:
	T _value;
};

} // namespace ast
