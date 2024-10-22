#include "renderer/vulkan_renderer.h"

#include <iostream>

namespace renderer {

renderer::renderer() {
	setup();
}

void renderer::setup() {
	std::cout << "renderer::setup()" << std::endl;
}

void renderer::update() {
	std::cout << "renderer::update()" << std::endl;
}

void renderer::render() {
	std::cout << "renderer::render()" << std::endl;
}

} // namespace renderer
