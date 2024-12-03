#include "ast/ast.h"
#include "ast/configuration.h"
#include "ast/utils.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_vulkan.h"

#include "parser.tab.h"
#include "renderer/vulkan_renderer.h"

#include <cstdlib>
#include <iostream>
#include <vector>

extern int yyparse();
extern FILE* yyin;
extern std::vector<ast::production_node*> vAst;
extern ast::configuration* config;

const char* current_file_path = nullptr;

// #define ENABLE_UI

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

	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	// Create window with Vulkan context
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+Vulkan example", nullptr, nullptr);
	if (!glfwVulkanSupported()) {
		printf("GLFW: Vulkan Not Supported\n");
		return 1;
	}

	ImVector<const char*> extensions;
	uint32_t extensions_count = 0;
	const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&extensions_count);
	for (uint32_t i = 0; i < extensions_count; i++)
		extensions.push_back(glfw_extensions[i]);
	SetupVulkan(extensions);

	// Create Window Surface
	VkSurfaceKHR surface;
	VkResult err = glfwCreateWindowSurface(VkData::Get()->g_Instance, window, VkData::Get()->g_Allocator, &surface);
	check_vk_result(err);

	// Create Framebuffers
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	ImGui_ImplVulkanH_Window* wd = &VkData::Get()->g_MainWindowData;
	SetupVulkanWindow(wd, surface, w, h);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForVulkan(window, true);
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = VkData::Get()->g_Instance;
	init_info.PhysicalDevice = VkData::Get()->g_PhysicalDevice;
	init_info.Device = VkData::Get()->g_Device;
	init_info.QueueFamily = VkData::Get()->g_QueueFamily;
	init_info.Queue = VkData::Get()->g_Queue;
	init_info.PipelineCache = VkData::Get()->g_PipelineCache;
	init_info.DescriptorPool = VkData::Get()->g_DescriptorPool;
	init_info.RenderPass = wd->RenderPass;
	init_info.Subpass = 0;
	init_info.MinImageCount = VkData::Get()->g_MinImageCount;
	init_info.ImageCount = wd->ImageCount;
	init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
	init_info.Allocator = VkData::Get()->g_Allocator;
	init_info.CheckVkResultFn = check_vk_result;
	ImGui_ImplVulkan_Init(&init_info);

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	renderer::renderer::get();

	// Main loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		// Resize swap chain?
		int fb_width, fb_height;
		glfwGetFramebufferSize(window, &fb_width, &fb_height);
		if (fb_width > 0 && fb_height > 0 && (VkData::Get()->g_SwapChainRebuild || VkData::Get()->g_MainWindowData.Width != fb_width || VkData::Get()->g_MainWindowData.Height != fb_height)) {
			ImGui_ImplVulkan_SetMinImageCount(VkData::Get()->g_MinImageCount);
			ImGui_ImplVulkanH_CreateOrResizeWindow(VkData::Get()->g_Instance,
				VkData::Get()->g_PhysicalDevice,
				VkData::Get()->g_Device,
				&VkData::Get()->g_MainWindowData,
				VkData::Get()->g_QueueFamily,
				VkData::Get()->g_Allocator,
				fb_width,
				fb_height,
				VkData::Get()->g_MinImageCount);
			VkData::Get()->g_MainWindowData.FrameIndex = 0;
			VkData::Get()->g_SwapChainRebuild = false;
		}
		if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
			ImGui_ImplGlfw_Sleep(10);
			continue;
		}

		// Start the Dear ImGui frame
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		{
			ImGui::Begin("Main");
			{
				const VkPhysicalDevice& gpu = VkData::Get()->g_PhysicalDevice;
				VkPhysicalDeviceProperties properties;
				vkGetPhysicalDeviceProperties(gpu, &properties);
				ImGui::Text("Selected GPU: %s", properties.deviceName);
			}

			ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			if (ImGui::Button("Quit")) {
				std::cout << "bye!" << std::endl;
				exit(0);
			}
			ImGui::End();
		}

		ImGui::Render();
		ImDrawData* draw_data = ImGui::GetDrawData();
		const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);
		if (!is_minimized) {
			wd->ClearValue.color.float32[0] = clear_color.x * clear_color.w;
			wd->ClearValue.color.float32[1] = clear_color.y * clear_color.w;
			wd->ClearValue.color.float32[2] = clear_color.z * clear_color.w;
			wd->ClearValue.color.float32[3] = clear_color.w;
			FrameRender(wd, draw_data);
			FramePresent(wd);
		}

		renderer::renderer::get()->update();
		renderer::renderer::get()->render();
	}

	// Cleanup
	err = vkDeviceWaitIdle(VkData::Get()->g_Device);
	check_vk_result(err);
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	CleanupVulkanWindow();
	CleanupVulkan();

	glfwDestroyWindow(window);
	glfwTerminate();

#endif

	return 0;
}
