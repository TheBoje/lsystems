#pragma once

#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

namespace renderer {

class renderer {
public:
	static inline renderer* get() {
		static renderer* _i = new renderer();
		return _i;
	}

	void run();
	~renderer();

private:
	renderer();
	[[nodiscard]] bool setup();
	void render();

private:
	GLFWwindow* _window = nullptr;
	ImGui_ImplVulkanH_Window* _wd = nullptr;
};

} // namespace renderer
