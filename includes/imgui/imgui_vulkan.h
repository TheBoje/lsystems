#pragma once

#include "imgui.h"
#include "imgui_impl_vulkan.h"

#include <stdio.h>	// printf, fprintf
#include <stdlib.h> // abort
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

struct VkData {
private:
public:
	static VkData* Get() {
		static VkData* _i = new VkData();
		return _i;
	}

	VkAllocationCallbacks* g_Allocator = nullptr;
	VkInstance g_Instance = VK_NULL_HANDLE;
	VkPhysicalDevice g_PhysicalDevice = VK_NULL_HANDLE;
	VkDevice g_Device = VK_NULL_HANDLE;
	uint32_t g_QueueFamily = (uint32_t)-1;
	VkQueue g_Queue = VK_NULL_HANDLE;
	VkDebugReportCallbackEXT g_DebugReport = VK_NULL_HANDLE;
	VkPipelineCache g_PipelineCache = VK_NULL_HANDLE;
	VkDescriptorPool g_DescriptorPool = VK_NULL_HANDLE;
	ImGui_ImplVulkanH_Window g_MainWindowData;
	int g_MinImageCount = 2;
	bool g_SwapChainRebuild = false;
};

void glfw_error_callback(int error, const char* description);
void check_vk_result(VkResult err);
VKAPI_ATTR VkBool32 VKAPI_CALL debug_report(
	VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData);
bool IsExtensionAvailable(const ImVector<VkExtensionProperties>& properties, const char* extension);
VkPhysicalDevice SetupVulkan_SelectPhysicalDevice();
void SetupVulkan(ImVector<const char*> instance_extensions);
void SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height);
void CleanupVulkan();
void CleanupVulkanWindow();
void FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data);
void FramePresent(ImGui_ImplVulkanH_Window* wd);
