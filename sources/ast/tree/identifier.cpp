#include "ast/tree/identifier.h"
#include "ast/tree/definition.h"
#include "ast/tree/definitions.h"
#include "expression.h"
#include <stdexcept>

namespace ast {

identifier::identifier(const std::string& value)
	: _value(value) { }

void identifier::print(std::ostream& stream) const {
	stream << _value;
}

bool identifier::operator==(const identifier& rhs) const {
	return _value == rhs._value;
}

abstract_expression* identifier::evaluate(std::vector<definition*> local_definitions, ast::definitions* global_definitions) const {
	auto get_identifier_value = [](ast::identifier id, std::vector<definition*> local_definitions, ast::definitions* global_definitions) -> ast::abstract_expression* {
		for (auto local_def : local_definitions) {
			if (local_def->_identifier->_value == id._value) {
				return local_def->_expression;
			}
		}

		for (auto global_node : global_definitions->_definition_list->_nodes) {
			auto global_def = static_cast<ast::definition*>(global_node);
			if (global_def->_identifier->_value == id._value) {
				return global_def->_expression;
			}
		}
		return nullptr;
	};

	auto value = get_identifier_value(*this, local_definitions, global_definitions);
	if (!value) {
		throw std::runtime_error("identifier not found");
	}

	if (dynamic_cast<ast::expression*>(value)) {
		return value->evaluate(local_definitions, global_definitions);
	}

	return value->clone();
}

identifier* identifier::clone() const {
	return new ast::identifier(*this);
}

} // namespace ast
