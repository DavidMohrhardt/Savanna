#pragma once

#include "SavannaVk.h"

#include "VkRendererCreateInfo.h"
#include "VkQueueFamilyIndices.h"

#include <vulkan/vulkan.h>

#include <string>
#include <unordered_set>
#include <vector>

namespace Savanna::Gfx::Vk::Utils
{
    void GetUniqueQueueFamilies(
        VkDeviceQueueCreateInfo *pQueueCreateInfos,
        uint32& queueCreateInfoCount,
        const QueueFamilyIndices& queueFamilyIndices,
        float* pQueuePriority = nullptr);

    void PopulateVkApplicationInfo(
        const RendererCreateInfo& rendererCreateInfo,
        VkApplicationInfo &applicationInfo);

    void PopulateVkInstanceCreateInfo(
        const RendererCreateInfo& rendererCreateInfo,
        const VkApplicationInfo* pApplicationCreateInfo,
        VkInstanceCreateInfo& instanceCreateInfo);

    void PopulateVkDeviceCreateInfo(
        const RendererCreateInfo& rendererCreateInfo,
        const VkDeviceQueueCreateInfo* queueCreateInfo,
        const uint32& queueCreateInfoCount,
        VkDeviceCreateInfo& deviceCreateInfo);
} // namespace Savanna::Gfx::Vk
