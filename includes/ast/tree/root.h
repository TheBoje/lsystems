#pragma once

#include "ast/tree/node.h"
#include "ast/tree/axiom.h"
#include "ast/tree/definitions.h"
#include "ast/tree/definition.h"
#include "ast/tree/productions.h"

#include "renderer/vulkan_renderer.h"

#include <unordered_map>
#include <utility>

namespace ast {
class root : public node {
public:
	root(node* axiom, node* definitions, node* productions);
	root(root&& other);
	root(const root& other);
	virtual ~root() override;
	virtual root& operator=(root&& rhs);
	virtual root& operator=(const root& rhs);
	virtual root* clone() const override;

	void print(std::ostream& stream) const override;

	node_list* derive();
	std::pair<std::vector<renderer::vertex>, std::vector<uint32_t>> get_vertices(ast::node_list* nodes);

private:
	[[nodiscard]] bool derive_checks() const;

	/**
  * @brief find an identifier of specific type and name in the definitions 
  *
  * @tparam TargetType required type 
  * @param name required name
  * @return true if definitions contains a matching `TargetType` and `name`
  */
	template <typename TargetType>
	bool has_identifier(const std::string& name) const {
		for (auto node : _definitions->_definition_list->_nodes) {
			auto definition = static_cast<ast::definition*>(node);
			// FIXME we are checking _expression's type directly, maybe try to find the _expression's result type
			if (typeid(*definition->_expression) == typeid(TargetType) && definition->_identifier->_value == name) {
				return true;
			}
		}
		return false;
	}

	ast::definition* get_definition(const std::string& name);

public:
	axiom* _axiom = nullptr;
	definitions* _definitions = nullptr;
	productions* _productions = nullptr;

private:
	std::unordered_map<std::string, ast::definition*> _hash_definitions;
};
} // namespace ast
