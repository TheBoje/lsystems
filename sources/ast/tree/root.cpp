#include "ast/tree/root.h"
#include "ast/tree/definitions.h"
#include "ast/tree/definition.h"
#include "ast/tree/productions.h"
#include "ast/tree/production.h"
#include "ast/tree/numerical.h"
#include "ast/tree/symbol.h"
#include "ast/tree/node_list.h"
#include "macros.h"

#include <algorithm>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <utility>
#include <string_view>

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <glm/trigonometric.hpp>

namespace ast {

root::root(node* axiom, node* definitions, node* productions)
	: _axiom(static_cast<ast::axiom*>(axiom))
	, _definitions(static_cast<ast::definitions*>(definitions))
	, _productions(static_cast<ast::productions*>(productions)) { }

root::root(root&& other)
	: node(std::move(other))
	, _axiom(other._axiom)
	, _definitions(other._definitions)
	, _productions(other._productions) {
	other._axiom = nullptr;
	other._definitions = nullptr;
	other._productions = nullptr;
}

root::root(const root& other)
	: node(other) {
	_axiom = new ast::axiom(*other._axiom);
	_definitions = new ast::definitions(*other._definitions);
	_productions = new ast::productions(*other._productions);
}

root::~root() {
	delete _axiom;
	delete _definitions;
	delete _productions;
}

root& root::operator=(root&& rhs) {
	if (this != &rhs) {
		node::operator=(std::move(rhs));

		delete _axiom;
		delete _definitions;
		delete _productions;

		_axiom = rhs._axiom;
		_definitions = rhs._definitions;
		_productions = rhs._productions;

		rhs._axiom = nullptr;
		rhs._definitions = nullptr;
		rhs._productions = nullptr;
	}

	return *this;
}

root& root::operator=(const root& rhs) {
	if (this != &rhs) {
		node::operator=(rhs);

		delete _axiom;
		delete _definitions;
		delete _productions;

		_axiom = rhs._axiom;
		_definitions = rhs._definitions;
		_productions = rhs._productions;
	}

	return *this;
}

root* root::clone() const {
	return new ast::root(*this);
}

void root::print(std::ostream& stream) const {
	stream << *_axiom << *_definitions << *_productions << std::endl;
}

/**
 * @brief Derive the L-System from the AST, must satisfy `root::derive_checks` requirements 
 *
 * @return node_list of all symbols from the L-System after 'd' derivations, might be nullptr
 */
node_list* root::derive() {
	if (derive_checks() == false) {
		return nullptr;
	}

	auto def_node = get_definition("d")->_expression;
	auto num_node = dynamic_cast<ast::numerical<int>*>(def_node);
	const int derivations = num_node->_value;
	const auto [max_left_context_size, max_right_context_size] = _productions->get_context_maxes();

	node_list* result = new node_list(_axiom->_symbols->_nodes);

	// For each derivation
	for (int step = 0; step < derivations; ++step) {
		node_list* new_result = new node_list();

		ast::node_list* prev_symbols = nullptr;
		ast::symbol* curr_symbol = nullptr;
		ast::node_list* next_symbols = nullptr;

		// For each symbol of the current step
		for (size_t symbol_index = 0; symbol_index < result->_nodes.size(); symbol_index++) {
			curr_symbol = static_cast<ast::symbol*>(result->_nodes[symbol_index]);
			// Previous context
			if (symbol_index > 0) {
				prev_symbols = new ast::node_list();

				for (size_t i = std::max((size_t)0, symbol_index - max_left_context_size); i < symbol_index; i++) {
					prev_symbols->push_back(result->_nodes.at(i)->clone());
				}

				//				for (auto begin = result->_nodes.begin(); begin != result->_nodes.begin() + symbol_index; begin++) {
				//					prev_symbols->_nodes.push_back((*begin)->clone());
				//				}
			} else {
				prev_symbols = nullptr;
			}
			// Next context
			if (symbol_index + 1 < result->_nodes.size()) {
				next_symbols = new ast::node_list();

				for (size_t i = std::min(result->_nodes.size() - 1, symbol_index + 1 + max_right_context_size); i < result->_nodes.size(); i++) {
					next_symbols->push_back(result->_nodes.at(i)->clone());
				}

				//				for (auto begin = result->_nodes.begin() + symbol_index + 1; begin != result->_nodes.end(); begin++) {
				//					next_symbols->_nodes.push_back((*begin)->clone());
				//				}
			} else {
				next_symbols = nullptr;
			}

			bool production_found = false;
			// Search for a viable production
			for (size_t production_index = 0; production_index < _productions->_production_list->_nodes.size(); ++production_index) {
				ast::production* current_production = static_cast<ast::production*>(_productions->_production_list->_nodes[production_index]);

				if (current_production->matches_symbol(curr_symbol, prev_symbols, next_symbols, _definitions) == false) {
					continue;
				}

				production_found = true;
				// add successor to new_result
				current_production->add_evaluated_successor(curr_symbol, prev_symbols, next_symbols, new_result, _definitions);

				break;
			}
			if (!production_found) {
				new_result->_nodes.push_back(curr_symbol->clone());
			}

			if (prev_symbols) {
				delete prev_symbols;
				prev_symbols = nullptr;
			}
			if (next_symbols) {
				delete next_symbols;
				next_symbols = nullptr;
			}
		}

		delete result;
		result = new node_list(*new_result);
		delete new_result;

		std::cout << "derivation " << step + 1 << "/" << derivations << ": " << *result << std::endl;
	}

	return result;
}

std::pair<std::vector<renderer::vertex>, std::vector<uint32_t>> root::get_vertices(ast::node_list* nodes) {
	struct State {
		glm::vec3 pos;
		glm::vec3 dir_forward;
		glm::vec3 dir_left;
		glm::vec3 dir_up;
		uint32_t index;
	};

	std::vector<renderer::vertex> vertices;
	std::vector<uint32_t> indices;
	const auto scale = glm::radians(static_cast<ast::numerical<double>*>(get_definition("s")->_expression)->_value);

	State state;
	state.pos = {0.0f, 0.0f, -1.0f};
	state.dir_forward = {0.0f, 0.0f, 1.0f};
	state.dir_left = {1.0f, 0.0f, 0.0f};
	state.dir_up = {0.0f, 1.0f, 0.0f};
	state.index = 0;

	std::stack<State> stack;
	const glm::vec3 color = {1.0f, 1.0f, 1.0f};
	vertices.push_back({state.pos, color});

	for (auto current_node : nodes->_nodes) {
		ast::symbol* current_symbol = dynamic_cast<ast::symbol*>(current_node);
		assert(current_symbol);

		const std::string& identifier_name = current_symbol->_identifier->_value;

		if (identifier_name == "F") {
			auto distance_expression = dynamic_cast<ast::abstract_expression*>(current_symbol->_args->_nodes[0]);
			assert(distance_expression);
			double distance = 0;

			auto result_expression = distance_expression->evaluate({}, nullptr);
			if (dynamic_cast<ast::numerical<double>*>(result_expression)) {
				distance = dynamic_cast<ast::numerical<double>*>(result_expression)->_value;
			} else {
				distance = dynamic_cast<ast::numerical<int>*>(result_expression)->_value;
			}

			glm::vec3 next = state.pos + state.dir_forward * (float)distance * (float)scale;

			vertices.push_back({next, color});
			uint32_t newIndex = vertices.size() - 1;

			indices.push_back(state.index);
			indices.push_back(newIndex);

			state.index = newIndex;
			state.pos = next;
		} else if (identifier_name == "P") {
			auto angle_expression = dynamic_cast<ast::abstract_expression*>(current_symbol->_args->_nodes[0]);
			assert(angle_expression);
			double angle = glm::radians(dynamic_cast<ast::numerical<double>*>(angle_expression->evaluate({}, nullptr))->_value);

			state.dir_forward = glm::rotate(state.dir_forward, (float)angle, state.dir_up);
			state.dir_left = glm::rotate(state.dir_left, (float)angle, state.dir_up);
		} else if (identifier_name == "M") {
			auto angle_expression = dynamic_cast<ast::abstract_expression*>(current_symbol->_args->_nodes[0]);
			assert(angle_expression);
			double angle = glm::radians(dynamic_cast<ast::numerical<double>*>(angle_expression->evaluate({}, nullptr))->_value);

			state.dir_forward = glm::rotate(state.dir_forward, -(float)angle, state.dir_up);
			state.dir_left = glm::rotate(state.dir_left, -(float)angle, state.dir_up);
		} else if (identifier_name == "SP") {
			stack.push(state);
		} else if (identifier_name == "Sp") {
			state = stack.top();
			stack.pop();

			vertices.push_back({state.pos, color});
			state.index = vertices.size() - 1;
		} else {
			// nothing
		}
	}

	return {vertices, indices};
}

/**
 * @brief performs checks on the current ast to make sure the L-System is derivable
 *
 * @details needs defines for `a` (angle), `s` (scale) and `d` (derivations)
 */
bool root::derive_checks() const {
	bool result = true;
	if ((result = has_identifier<ast::numerical<double>>("a")) == false) {
		std::cout << "error: missing definition for `a` (float) for angle value" << std::endl;
	}

	if ((result = has_identifier<ast::numerical<double>>("s")) == false) {
		std::cout << "error: missing definition for `s` (float) for scale value" << std::endl;
	}

	if ((result = has_identifier<ast::numerical<int>>("d")) == false) {
		std::cout << "error: missing definition for `d` (int) for derivations" << std::endl;
	}

	return result;
}

ast::definition* root::get_definition(const std::string& name) {
	if (_hash_definitions.contains(name)) {
		return _hash_definitions.find(name)->second;
	}

	for (auto node : _definitions->_definition_list->_nodes) {
		auto definition = static_cast<ast::definition*>(node);
		if (definition->_identifier->_value == name) {
			_hash_definitions.insert({name, definition});
			return definition;
		}
	}

	return nullptr;
}

} // namespace ast
