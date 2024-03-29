#pragma once

#include <Utilities/SavannaCoding.h>

#include "Public/ISavannaGfxVk2.h"

#include <vulkan/vulkan.h>

#define SAVANNA_VK_APPLICATION_NAME "Savanna"
#define SAVANNA_VK_ENGINE_NAME "Savanna Engine"
#define SAVANNA_VK_API_VERSION VK_API_VERSION_1_3
#define SAVANNA_VK_APPLICATION_VERSION VK_MAKE_VERSION(0, 1, 0)

namespace Savanna::Gfx::Vk2::Utils
{
    constexpr VkApplicationInfo k_SavannaDefaultVulkanAppInfo
    {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = nullptr,
        .pApplicationName = SAVANNA_VK_APPLICATION_NAME,
        .applicationVersion = SAVANNA_VK_APPLICATION_VERSION,
        .pEngineName = SAVANNA_VK_ENGINE_NAME,
        .engineVersion = SAVANNA_VK_APPLICATION_VERSION,
        .apiVersion = SAVANNA_VK_API_VERSION
    };

    void PopulateInstanceCreateInfo(
        VkInstanceCreateInfo& outCreateInfo,
        const char** ppEnabledInstanceExtensions = nullptr,
        uint32 enabledInstanceExtensionCount = 0,
        const char** ppEnabledLayers = nullptr,
        uint32 enabledLayerCount = 0);

    void PopulateVkDeviceCreateInfo(
        const se_VkDriverCreateInfo_t& createInfo,
        VkDeviceCreateInfo& deviceCreateInfo);

    void PopulateVkDeviceCreateInfo(
        const se_VkDriverCreateInfo_t& createInfo,
        const VkDeviceQueueCreateInfo* pQueueCreateInfo,
        const uint32 queueCreateInfoCount,
        VkDeviceCreateInfo& deviceCreateInfo);
} // namespace Savanna::Gfx::Vk
