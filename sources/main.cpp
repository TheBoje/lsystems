#include "ast/ast.h"
#include "ast/configuration.h"
#include "ast/utils.h"

#include "imgui_impl_glfw.h"
#include "imgui_vulkan.h"

#include "parser.tab.h"

#include <iostream>
#include <vector>

extern int yyparse();
extern std::vector<ast::production_node*> vAst;
extern ast::configuration* config;

int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;

	yyparse();

	if (vAst.size() <= 0) {
		std::cout << "nodes: none" << std::endl;
	}

	for (ast::node* node : vAst) {
		std::cout << "node: " << *node << std::endl;
	}

	std::cout << "config: " << std::endl << *config << std::endl;

	std::cout << ast::utils::derive_lsystem(config, vAst) << std::endl;

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
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	// ImGui::StyleColorsLight();

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

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can
	// also load multiple fonts and use ImGui::PushFont()/PopFont() to select
	// them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you
	// need to select the font among multiple.
	// - If the file cannot be loaded, the function will return a nullptr. Please
	// handle those errors in your application (e.g. use an assertion, or display
	// an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored
	// into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which
	// ImGui_ImplXXXX_NewFrame below will call.
	// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype
	// for higher quality font rendering.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string
	// literal you need to write a double backslash \\ !
	// io.Fonts->AddFontDefault();
	// io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
	// io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	// io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	// io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	// ImFont* font =
	// io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f,
	// nullptr, io.Fonts->GetGlyphRangesJapanese()); IM_ASSERT(font != nullptr);

	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Main loop
	while (!glfwWindowShouldClose(window)) {
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to
		// tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to
		// your main application, or clear/overwrite your copy of the mouse data.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input
		// data to your main application, or clear/overwrite your copy of the
		// keyboard data. Generally you may always pass all inputs to dear imgui,
		// and hide them from your application based on those two flags.
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

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text."); // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window",
				&show_demo_window); // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);			// Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}

		// 3. Show another simple window.
		if (show_another_window) {
			ImGui::Begin("Another Window",
				&show_another_window); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}

		// Rendering
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

	return 0;
}
