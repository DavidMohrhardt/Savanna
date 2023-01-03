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
        VkDeviceQueueCreateInfo* pQueueCreateInfos,
        uint32_t* queueCreateInfoCount,
        const se_VkRendererCreateInfo_t* const pRendererCreateInfo,
        const QueueFamilyIndices& queueFamilyIndices,
        float* pQueuePriority);

    void PopulateVkApplicationInfo(VkApplicationInfo &applicationInfo, const RendererCreateInfo* const pRendererCreateInfo);

    void PopulateVkInstanceCreateInfo(
        VkInstanceCreateInfo* pInstanceCreateInfo,
        const se_VkRendererCreateInfo_t* const pRendererCreateInfo,
        VkApplicationInfo* pApplicationCreateInfo = nullptr);

    void PopulateVkDeviceCreateInfo(
        VkDeviceCreateInfo* pDeviceCreateInfo,
        const se_VkRendererCreateInfo_t* const pRendererCreateInfo,
        const VkDeviceQueueCreateInfo* pQueueCreateInfos,
        uint32 queueCreateInfoCount);

    // void PopulateVkDeviceQueueCreateInfo(VkDeviceQueueCreateInfo& deviceQueueCreateInfo, const se_VkRendererCreateInfo_t* const pRendererCreateInfo);

    void PopulateVkSwapchainCreateInfo(VkSwapchainCreateInfoKHR& swapchainCreateInfo, const se_VkRendererCreateInfo_t* const pRendererCreateInfo);

    void PopulateVkPresentInfoKHR(VkPresentInfoKHR& presentInfo, const se_VkRendererCreateInfo_t* const pRendererCreateInfo);
} // namespace Savanna::Gfx::Vk
