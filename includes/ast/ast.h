#pragma once

#include <iostream>
#include <ostream>
#include <string>

namespace ast {

class node {
public:
	virtual ~node() { }
	virtual void print(std::ostream& stream) const = 0;

	friend std::ostream& operator<<(std::ostream& os, const node& node) {
		node.print(os);
		return os;
	}
};

class context_node : public node {
public:
	context_node(const std::string& symbols)
		: context_symbols(symbols) { }

	void print(std::ostream& stream) const override {
		stream << context_symbols;
	}

	bool operator==(const context_node& other) const {
		return context_symbols == other.context_symbols;
	}
	bool operator!=(const context_node& other) const {
		return !(*this == other);
	}

public:
	std::string context_symbols;
};

class symbol_node : public node {
public:
	symbol_node(const std::string& symbol)
		: symbol(symbol) { }

	void print(std::ostream& stream) const override {
		stream << symbol;
	}

	bool operator==(const symbol_node& other) const {
		return symbol == other.symbol;
	}
	bool operator!=(const symbol_node& other) const {
		return !(*this == other);
	}

public:
	std::string symbol;
};

class replacement_node : public node {
public:
	replacement_node(symbol_node* symbols)
		: result_symbol(symbols) { }

	void print(std::ostream& stream) const override {
		stream << *result_symbol;
	}

	bool operator==(const replacement_node& other) const {
		return result_symbol == other.result_symbol;
	}
	bool operator!=(const replacement_node& other) const {
		return !(*this == other);
	}

public:
	symbol_node* result_symbol;
};

class production_node : public node {
public:
	production_node(context_node* left, symbol_node* symbol, context_node* right, replacement_node* replacement)
		: left_context(left)
		, right_context(right)
		, symbol_node(symbol)
		, replacement_node(replacement) { }

	void print(std::ostream& stream) const override {
		if (left_context)
			stream << *left_context;
		else
			stream << "*";

		stream << " < " << *symbol_node << " > ";

		if (right_context)
			stream << *right_context;
		else
			stream << "*";

		stream << " --> " << *replacement_node;
	}
	bool operator==(const production_node& other) const {
		return left_context == other.left_context && right_context == other.right_context && symbol_node == other.symbol_node && replacement_node == other.replacement_node;
	}
	bool operator!=(const production_node& other) const {
		return !(*this == other);
	}

public:
	context_node* left_context = nullptr;
	context_node* right_context = nullptr;
	symbol_node* symbol_node = nullptr;
	replacement_node* replacement_node = nullptr;
};

} // namespace ast
