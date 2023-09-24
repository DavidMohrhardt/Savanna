#pragma once

#include "SavannaVk.h"

#include "VkRendererCreateInfo.h"
#include "VkQueueFamilyIndices.h"

#include <vulkan/vulkan.h>

#include <string>
#include <unordered_set>
#include <vector>

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

    // void GetUniqueVkQueueFamilies(
    //     VkDeviceQueueCreateInfo *pQueueCreateInfos,
    //     uint32& queueCreateInfoCount,
    //     const QueueFamilyIndices& queueFamilyIndices,
    //     float* pQueuePriority = nullptr);

    // void PopulateVkApplicationInfo(
    //     const RendererCreateInfo& rendererCreateInfo,
    //     VkApplicationInfo &applicationInfo);

    // void PopulateVkInstanceCreateInfo(
    //     const RendererCreateInfo& rendererCreateInfo,
    //     const VkApplicationInfo* pApplicationCreateInfo,
    //     VkInstanceCreateInfo& instanceCreateInfo);

    // void PopulateVkDeviceCreateInfo(
    //     const RendererCreateInfo& rendererCreateInfo,
    //     const VkDeviceQueueCreateInfo* queueCreateInfo,
    //     const uint32& queueCreateInfoCount,
    //     VkDeviceCreateInfo& deviceCreateInfo);
} // namespace Savanna::Gfx::Vk
