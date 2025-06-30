#include "ast/tree/productions.h"

#include "ast/tree/production.h"
#include <cassert>

namespace ast {

productions::productions(node_list* production_lst)
	: _production_list(static_cast<ast::production_list*>(production_lst)) { }

productions::productions(productions&& other)
	: node(std::move(other))
	, _production_list(other._production_list) {
	other._production_list = nullptr;
}

productions::productions(const productions& other)
	: node(other) {
	if (other._production_list) {
		_production_list = new production_list(*other._production_list); // deep copy
	}
}

productions::~productions() {
	delete _production_list;
	_production_list = nullptr;
}

productions& productions::operator=(productions&& other) {
	if (this != &other) {
		node::operator=(std::move(other));
		delete _production_list;
		_production_list = nullptr;
		if (other._production_list) {
			_production_list = new ast::production_list(*other._production_list);
			other._production_list = nullptr;
		}
	}
	return *this;
}

productions& productions::operator=(const productions& other) {
	if (this != &other) {
		node::operator=(other);
		delete _production_list;
		_production_list = nullptr;
		if (other._production_list) {
			_production_list = new ast::production_list(*other._production_list);
		}
	}
	return *this;
}

productions* productions::clone() const {
	return new ast::productions(*this);
}

void productions::print(std::ostream& stream) const {
	stream << "productions:" << *_production_list;
}

std::pair<size_t, size_t> productions::get_context_maxes() const {
	size_t left_max = 0;
	size_t right_max = 0;

	for (auto node : _production_list->_nodes) {
		auto production = dynamic_cast<ast::production*>(node);
		assert(production);

		if (production->_predecessor->_left_context) {
			left_max = std::max(left_max, production->_predecessor->_left_context->_nodes.size());
		}

		if (production->_predecessor->_right_context) {
			right_max = std::max(right_max, production->_predecessor->_right_context->_nodes.size());
		}
	}

	return {left_max, right_max};
}

} // namespace ast
