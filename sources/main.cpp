#include "ast/utils.h"
#include "ast/tree/node.h"
#include "ast/tree/root.h"

#include "utils/macros.h"
#include "parser.tab.h"
#include "renderer/vulkan_renderer.h"

#include <cstdlib>
#include <exception>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

extern int yyparse();
extern FILE* yyin;
extern ast::node* Ast;

const char* current_file_path = nullptr;

#define ENABLE_UI
#ifdef TEST
	#undef ENABLE_UI
#endif

int main(int argc, char* argv[]) {
	if (argc >= 2) {
		current_file_path = argv[1];
	} else {
		current_file_path = "examples/default.lsy";
	}

	FILE* file = fopen(current_file_path, "r");
	if (!file) {
		std::cerr << "Error: Could not open file " << current_file_path << std::endl;
		return EXIT_FAILURE;
	}
	yyin = file;
	int res = yyparse();
	fclose(file);

	if (res != EXIT_SUCCESS) {
		return res;
	}

	if (Ast) {
		std::cout << *Ast << std::endl;
	} else {
		std::cout << "nodes: none" << std::endl;
	}

	ast::root* root = static_cast<ast::root*>(Ast);
	auto result = root->derive();
	std::cout << "result: " << *result << std::endl;

	// std::cout << "config: " << std::endl << *config << std::endl;
	//std::cout << ast::utils::derive_lsystem(config, vAst) << std::endl;

#ifdef ENABLE_UI

	const auto& [vertices, indices] = root->get_vertices(result);

	renderer::renderer::get()->setVertices(vertices, indices);

	try {
		renderer::renderer::get()->run();
	} catch (const std::exception& e) {
		std::cerr << "Exception while running: " << e.what() << std::endl;
	} catch (...) {
		std::cerr << "Error while running." << std::endl;
	}

	std::cout << "bye!" << std::endl;
#endif

	return EXIT_SUCCESS;
}
