#include "ast/tree/predecessor.h"

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

bool predecessor::matches_context_left(ast::node_list* context) const {
	// TODO:
	return true;
}
bool predecessor::matches_context_right(ast::node_list* context) const {
	// TODO:
	return true;
}

} // namespace ast
