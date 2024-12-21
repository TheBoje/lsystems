#include "renderer/vulkan_renderer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_vulkan.h"

#include <vulkan/vulkan.hpp>

#include <stdexcept>
#include <iostream>

namespace renderer {

renderer::renderer() {
	if (!setup()) {
		throw std::runtime_error("rendered failed to setup");
	}
}

renderer::~renderer() {
	VkResult err = vkDeviceWaitIdle(VkData::Get()->g_Device);
	check_vk_result(err);
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	CleanupVulkanWindow();
	CleanupVulkan();

	glfwDestroyWindow(_window);
	glfwTerminate();
}

bool renderer::setup() {
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return false;

	// Create window with Vulkan context
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	_window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+Vulkan example", nullptr, nullptr);
	if (!glfwVulkanSupported()) {
		printf("GLFW: Vulkan Not Supported\n");
		return false;
	}

	ImVector<const char*> extensions;
	uint32_t extensions_count = 0;
	const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&extensions_count);
	for (uint32_t i = 0; i < extensions_count; i++)
		extensions.push_back(glfw_extensions[i]);
	SetupVulkan(extensions);

	// Create Window Surface
	VkSurfaceKHR surface;
	VkResult err = glfwCreateWindowSurface(VkData::Get()->g_Instance, _window, VkData::Get()->g_Allocator, &surface);
	check_vk_result(err);

	// Create Framebuffers
	int w, h;
	glfwGetFramebufferSize(_window, &w, &h);
	_wd = &VkData::Get()->g_MainWindowData;
	SetupVulkanWindow(_wd, surface, w, h);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForVulkan(_window, true);
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = VkData::Get()->g_Instance;
	init_info.PhysicalDevice = VkData::Get()->g_PhysicalDevice;
	init_info.Device = VkData::Get()->g_Device;
	init_info.QueueFamily = VkData::Get()->g_QueueFamily;
	init_info.Queue = VkData::Get()->g_Queue;
	init_info.PipelineCache = VkData::Get()->g_PipelineCache;
	init_info.DescriptorPool = VkData::Get()->g_DescriptorPool;
	init_info.RenderPass = _wd->RenderPass;
	init_info.Subpass = 0;
	init_info.MinImageCount = VkData::Get()->g_MinImageCount;
	init_info.ImageCount = _wd->ImageCount;
	init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
	init_info.Allocator = VkData::Get()->g_Allocator;
	init_info.CheckVkResultFn = check_vk_result;
	ImGui_ImplVulkan_Init(&init_info);

	return true;
}

void renderer::run() {
	while (!glfwWindowShouldClose(_window)) {
		glfwPollEvents();

		// Resize swap chain?
		int fb_width, fb_height;
		glfwGetFramebufferSize(_window, &fb_width, &fb_height);
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
		if (glfwGetWindowAttrib(_window, GLFW_ICONIFIED) != 0) {
			ImGui_ImplGlfw_Sleep(10);
			continue;
		}

		render();
	}
}

void renderer::render() {
	const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImGuiIO& io = ImGui::GetIO();

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
		_wd->ClearValue.color.float32[0] = clear_color.x * clear_color.w;
		_wd->ClearValue.color.float32[1] = clear_color.y * clear_color.w;
		_wd->ClearValue.color.float32[2] = clear_color.z * clear_color.w;
		_wd->ClearValue.color.float32[3] = clear_color.w;
		FrameRender(_wd, draw_data);
		FramePresent(_wd);
	}
}

} // namespace renderer
