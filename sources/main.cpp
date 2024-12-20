#include "ast/ast.h"
#include "ast/configuration.h"
#include "ast/utils.h"

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
extern std::vector<ast::production_node*> vAst;
extern ast::configuration* config;

const char* current_file_path = nullptr;

#define ENABLE_UI

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
		return EXIT_FAILURE;
	}

	current_file_path = argv[1];
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

	if (vAst.size() <= 0) {
		std::cout << "nodes: none" << std::endl;
	}

	for (ast::node* node : vAst) {
		std::cout << "node: " << *node << std::endl;
	}

	std::cout << "config: " << std::endl << *config << std::endl;
	std::cout << ast::utils::derive_lsystem(config, vAst) << std::endl;

#ifdef ENABLE_UI

	glm::mat4 mat;
	glm::vec4 vec;
	glm::vec4 res_mat = mat * vec;
	UNUSED(res_mat);

	auto* renderer = renderer::renderer::get();

	try {
		renderer->run();
	} catch (const std::exception& e) {
		std::cerr << "Exception while running: " << e.what() << std::endl;
	} catch (...) {
		std::cerr << "Error while running." << std::endl;
	}

#endif

	return 0;
}
