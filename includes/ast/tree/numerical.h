#pragma once

#include "ast/tree/node.h"

namespace ast {

template <typename T>
class numerical : public node {
public:
	numerical(T value)
		: _value(value) { }
	void print(std::ostream& stream) const override {
		stream << _value;
	}

public:
	T _value;
};

} // namespace ast
