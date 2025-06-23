#include "ast/tree/definitions.h"
#include "ast/tree/definition_list.h"

namespace ast {

definitions::definitions(node_list* definition_list)
	: _definition_list(static_cast<ast::definition_list*>(definition_list)) { }

definitions::definitions(definitions&& other)
	: node(std::move(other))
	, _definition_list(other._definition_list) {
	other._definition_list = nullptr;
}

definitions::definitions(const definitions& other)
	: node(other) {
	_definition_list = new definition_list(*other._definition_list); // deep copy
}

definitions::~definitions() {
	delete _definition_list;
	_definition_list = nullptr;
}

definitions& definitions::operator=(definitions&& other) {
	if (this != &other) {
		node::operator=(std::move(other));
		delete _definition_list;
		_definition_list = nullptr;
		_definition_list = new ast::definition_list(*other._definition_list);
		other._definition_list = nullptr;
	}
	return *this;
}

definitions& definitions::operator=(const definitions& other) {
	if (this != &other) {
		node::operator=(other);
		delete _definition_list;
		_definition_list = nullptr;
		_definition_list = new ast::definition_list(*other._definition_list);
	}
	return *this;
}

definitions* definitions::clone() const {
	return new ast::definitions(*this);
}

void definitions::print(std::ostream& stream) const {
	stream << "definitions: " << *_definition_list << std::endl;
}

} // namespace ast
