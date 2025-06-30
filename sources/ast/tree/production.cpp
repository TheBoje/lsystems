#include "ast/tree/production.h"
#include "abstract_expression.h"
#include "ast/tree/successor.h"
#include "ast/tree/definition.h"

#include "macros.h"
#include <cassert>
#include <stdexcept>

namespace ast {

production::production(node* predecessor, node* successor, node* condition)
	: _predecessor(static_cast<ast::predecessor*>(predecessor))
	, _condition(static_cast<ast::condition*>(condition))
	, _successor(static_cast<ast::successor*>(successor)) { }

production::production(production&& other)
	: node(std::move(other))
	, _predecessor(other._predecessor)
	, _condition(other._condition)
	, _successor(other._successor) {
	other._predecessor = nullptr;
	other._condition = nullptr;
	other._successor = nullptr;
}

production::production(const production& other)
	: node(other) {
	_predecessor = new ast::predecessor(*other._predecessor);
	_condition = new ast::condition(*other._condition);
	_successor = new ast::successor(*other._successor);
}

production::~production() {
	delete _predecessor;
	delete _condition;
	delete _successor;
}

production& production::operator=(production&& rhs) {
	if (this != &rhs) {
		node::operator=(std::move(rhs));

		delete _predecessor;
		delete _condition;
		delete _successor;

		_predecessor = rhs._predecessor;
		_condition = rhs._condition;
		_successor = rhs._successor;

		rhs._predecessor = nullptr;
		rhs._condition = nullptr;
		rhs._successor = nullptr;
	}

	return *this;
}

production& production::operator=(const production& rhs) {
	if (this != &rhs) {
		node::operator=(rhs);

		delete _predecessor;
		delete _condition;
		delete _successor;

		_predecessor = rhs._predecessor;
		_condition = rhs._condition;
		_successor = rhs._successor;
	}

	return *this;
}

void production::print(std::ostream& stream) const {
	stream << *_predecessor;

	if (_condition) {
		stream << " : " << *_condition;
	}

	stream << " --> " << *_successor;
}

bool production::matches_symbol(ast::symbol* symbol, ast::node_list* left_context, ast::node_list* right_context, ast::definitions* definitions) const {
	if (_predecessor->matches_symbol(symbol, left_context, right_context) == false) {
		return false;
	}

	if (satisfy_conditions(symbol, left_context, right_context, definitions) == false) {
		return false;
	}

	return true;
}

void production::add_evaluated_successor(ast::symbol* source_symbol, ast::node_list* left_context, ast::node_list* right_context, ast::node_list* result, ast::definitions* definitions) const {
	std::vector<ast::definition*> local_definitions = _predecessor->get_local_definitions(source_symbol, left_context, right_context);

	for (auto node : _successor->_symbols->_nodes) {
		auto clone = static_cast<ast::symbol*>(node)->clone();

		if (clone->_args) {
			for (size_t i = 0; i < clone->_args->_nodes.size(); i++) {
				auto node_arg = static_cast<ast::abstract_expression*>(clone->_args->_nodes[i]);

				ast::abstract_expression* result = node_arg->evaluate(local_definitions, definitions);
				if (!result) {
					throw std::runtime_error("evaluation failed");
				}

				clone->_args->_nodes[i] = result->clone();
			}
		}

		result->_nodes.push_back(clone);
	}

	for (auto local_definition : local_definitions) {
		delete local_definition;
	}
}

production* production::clone() const {
	return new ast::production(*this);
}

bool production::satisfy_conditions(ast::symbol* symbol, ast::node_list* left_context, ast::node_list* right_context, ast::definitions* definitions) const {
	if (!_condition) {
		return true;
	}

	std::vector<ast::definition*> local_definitions = _predecessor->get_local_definitions(symbol, left_context, right_context);

	bool result = _condition->evaluate(local_definitions, definitions);

	for (auto local_definition : local_definitions) {
		delete local_definition;
	}

	return result;
}

} // namespace ast
