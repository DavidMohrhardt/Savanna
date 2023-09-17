#include "VkDriver.h"

void Savanna::Gfx::Vk::VkDriver::CreateInstance()
{
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Savanna";
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
    appInfo.pEngineName = "Savanna";
    appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
    appInfo.apiVersion = VK_API_VERSION_1_2;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    if (vkCreateInstance(&createInfo, nullptr, &m_Instance) != VK_SUCCESS)
    {
        SAVANNA_FATAL_LOG("Failed to create Vulkan instance!");
    }

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    DynamicArray<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.Data());

    // SAVANNA_INFO_LOG("Available Vulkan extensions:");
    // for (const auto& extension : extensions)
    // {
    //     SAVANNA_INFO_LOG("\t{}", extension.extensionName);
    // }
}

void Savanna::Gfx::Vk::VkDriver::CreateSurface() {}

void Savanna::Gfx::Vk::VkDriver::PickPhysicalDevice() {}

void Savanna::Gfx::Vk::VkDriver::CreateLogicalDevice() {}

void Savanna::Gfx::Vk::VkDriver::CreateSwapchain() {}

void Savanna::Gfx::Vk::VkDriver::CreateImageViews() {}

void Savanna::Gfx::Vk::VkDriver::CreateRenderPass() {}

void Savanna::Gfx::Vk::VkDriver::CreateGraphicsPipeline() {}

void Savanna::Gfx::Vk::VkDriver::CreateFramebuffers() {}

void Savanna::Gfx::Vk::VkDriver::CreateCommandBuffers() {}

void Savanna::Gfx::Vk::VkDriver::CreateSyncObjects() {}

void Savanna::Gfx::Vk::VkDriver::CleanupSwapchain() {}
