#include "ast/tree/predecessor.h"
#include "ast/tree/definition.h"
#include "symbol.h"
#include <cassert>

namespace ast {

predecessor::predecessor(ast::node* symbol, node_list* left_ctx, node_list* right_ctx)
	: _symbol(static_cast<ast::symbol*>(symbol))
	, _left_context(left_ctx)
	, _right_context(right_ctx) { }

void predecessor::print(std::ostream& stream) const {
	if (_left_context) {
		stream << *_left_context << " < ";
	}

	stream << *_symbol;

	if (_right_context) {
		stream << " > " << *_right_context;
	}
}

predecessor::predecessor(predecessor&& other)
	: node(std::move(other))
	, _symbol(other._symbol)
	, _left_context(other._left_context)
	, _right_context(other._right_context) {
	other._symbol = nullptr;
	other._left_context = nullptr;
	other._right_context = nullptr;
}

predecessor::predecessor(const predecessor& other)
	: node(other) {
	_symbol = new ast::symbol(*other._symbol);

	if (other._left_context) {
		_left_context = new ast::node_list(*other._left_context);
	}

	if (other._right_context) {
		_right_context = new ast::node_list(*other._right_context);
	}
}

predecessor::~predecessor() {
	delete _symbol;

	if (_left_context) {
		delete _left_context;
		_left_context = nullptr;
	}

	if (_right_context) {
		delete _right_context;
		_right_context = nullptr;
	}
}

predecessor& predecessor::operator=(predecessor&& rhs) {
	if (this != &rhs) {
		node::operator=(std::move(rhs));

		delete _symbol;

		if (_left_context) {
			delete _left_context;
		}

		if (_right_context) {
			delete _right_context;
		}

		_symbol = rhs._symbol;
		_left_context = rhs._left_context;
		_right_context = rhs._right_context;

		rhs._symbol = nullptr;
		rhs._left_context = nullptr;
		rhs._right_context = nullptr;
	}

	return *this;
}

predecessor& predecessor::operator=(const predecessor& rhs) {
	if (this != &rhs) {
		node::operator=(rhs);

		delete _symbol;

		if (_left_context) {
			delete _left_context;
		}

		if (_right_context) {
			delete _right_context;
		}

		_symbol = rhs._symbol;
		_left_context = rhs._left_context;
		_right_context = rhs._right_context;
	}

	return *this;
}

predecessor* predecessor::clone() const {
	return new ast::predecessor(*this);
}

bool predecessor::matches_symbol(ast::symbol* symbol, ast::node_list* left_context, ast::node_list* right_context) const {
	if (*symbol->_identifier != *_symbol->_identifier) {
		return false;
	}

	if (_left_context) {
		if (!left_context || left_context->_nodes.empty()) {
			return false;
		}

		if (matches_context_left(left_context) == false) {
			return false;
		}
	}

	if (_right_context) {
		if (!right_context || right_context->_nodes.empty()) {
			return false;
		}

		if (matches_context_right(right_context) == false) {
			return false;
		}
	}

	return true;
}

std::vector<ast::definition*> predecessor::get_local_definitions(ast::symbol* symbol, ast::node_list* left_context, ast::node_list* right_context) const {
	std::vector<ast::definition*> result {};

	auto add_args_to_definition = [&result](ast::symbol* source_symbol, ast::symbol* predecessor_symbol) -> void {
		if (source_symbol->_args) {
			assert(predecessor_symbol->_args);
			assert(predecessor_symbol->_args->_nodes.size() == source_symbol->_args->_nodes.size());

			for (size_t i = 0; i < source_symbol->_args->_nodes.size(); i++) {
				auto predecessor_identifier = predecessor_symbol->_args->_nodes[i];
				auto source_value = source_symbol->_args->_nodes[i];

				auto local_definition = new ast::definition(predecessor_identifier->clone(), source_value->clone());
				result.push_back(local_definition);
			}
		}
	};

	add_args_to_definition(symbol, _symbol);

	if (_left_context && _left_context->_nodes.size() > 0) {
		assert(left_context);

		size_t size = std::min(_left_context->_nodes.size(), left_context->_nodes.size());
		for (size_t i = size - 1; i > 0; i--) {
			auto input_left_symbol = dynamic_cast<ast::symbol*>(left_context->_nodes[i]);
			assert(input_left_symbol);
			auto predecessor_left_symbol = dynamic_cast<ast::symbol*>(_left_context->_nodes[i]);
			assert(predecessor_left_symbol);

			add_args_to_definition(input_left_symbol, predecessor_left_symbol);
		}
	}

	if (_right_context && _right_context->_nodes.size() > 0) {
		assert(right_context);

		size_t size = std::min(_right_context->_nodes.size(), right_context->_nodes.size());
		for (size_t i = 0; i < size; i++) {
			auto input_right_symbol = dynamic_cast<ast::symbol*>(right_context->_nodes[i]);
			assert(input_right_symbol);
			auto predecessor_right_symbol = dynamic_cast<ast::symbol*>(_right_context->_nodes[i]);
			assert(predecessor_right_symbol);

			add_args_to_definition(input_right_symbol, predecessor_right_symbol);
		}
	}

	return result;
}

bool predecessor::matches_context_left(ast::node_list* context) const {
	if (!_left_context || _left_context->_nodes.empty()) {
		return true;
	}
	assert(context);

	if (context->_nodes.size() < _left_context->_nodes.size()) {
		return false;
	}

	const size_t size_reference_context = _left_context->_nodes.size();
	const size_t size_arg_context = context->_nodes.size();
	for (size_t i = 0; i < size_reference_context; i++) {
		auto input_left_symbol = dynamic_cast<ast::symbol*>(context->_nodes[size_arg_context - 1 - i]);
		assert(input_left_symbol);
		auto predecessor_left_symbol = dynamic_cast<ast::symbol*>(_left_context->_nodes[size_reference_context - 1 - i]);
		assert(predecessor_left_symbol);

		if (input_left_symbol->_identifier->_value != predecessor_left_symbol->_identifier->_value) {
			return false;
		}

		if (!input_left_symbol->_args && predecessor_left_symbol->_args) {
			return false;
		}

		if (input_left_symbol->_args && !predecessor_left_symbol->_args) {
			return false;
		}

		if (input_left_symbol->_args) {
			if (input_left_symbol->_args->_nodes.size() != predecessor_left_symbol->_args->_nodes.size()) {
				return false;
			}
		}
	}

	return true;
}

bool predecessor::matches_context_right(ast::node_list* context) const {
	if (!_right_context || _right_context->_nodes.empty()) {
		return true;
	}
	assert(context);

	if (context->_nodes.size() < _right_context->_nodes.size()) {
		return false;
	}

	for (size_t i = 0; i <= _right_context->_nodes.size(); i++) {
		auto input_right_symbol = dynamic_cast<ast::symbol*>(context->_nodes[i]);
		assert(input_right_symbol);
		auto predecessor_right_symbol = dynamic_cast<ast::symbol*>(_right_context->_nodes[i]);
		assert(predecessor_right_symbol);

		if (input_right_symbol->_identifier->_value != predecessor_right_symbol->_identifier->_value) {
			return false;
		}

		if (!input_right_symbol->_args && predecessor_right_symbol->_args) {
			return false;
		}

		if (input_right_symbol->_args && !predecessor_right_symbol->_args) {
			return false;
		}

		if (input_right_symbol->_args) {
			if (input_right_symbol->_args->_nodes.size() != predecessor_right_symbol->_args->_nodes.size()) {
				return false;
			}
		}
	}

	return true;
}

} // namespace ast
