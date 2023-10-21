#include "VkGpu.h"

#include "VkQueueFamilyIndices.h"

#include "Utilities/VkPhysicalDeviceSelectionUtils.h"
#include "Utilities/VkInfoCreateUtils.h"
#include "Utilities/VkExtensionUtils.h"
#include "Utilities/VkResultUtils.h"

#include "Memory/MemoryManager.h"

#include <unordered_set>

namespace Savanna::Gfx::Vk2
{
    bool VkGpu::InitializeLogicalDevice(
        const se_VkDriverCreateInfo_t &createInfo,
        VkInstance& instance,
        VkSurfaceKHR& surface,
        const VkAllocationCallbacks* pAllocationCallbacks)
    {
        VkQueueFamilyIndices queueFamilyIndices(m_PhysicalDevice, surface);
        uint32 queueCreateInfoCount = queueFamilyIndices.GetQueueFamilyCount();
        dynamic_array<VkDeviceQueueCreateInfo> queueCreateInfos(queueCreateInfoCount, k_SavannaMemoryArenaIdGfx);
        float queuePriority = 1.0f;
        queueFamilyIndices.GetUniqueQueueFamilies(queueCreateInfos, &queuePriority);

        dynamic_array<const char*> enabledDeviceExtensions { createInfo.m_LogicalDeviceCreateArgs.m_EnabledDeviceExtensionCount, k_SavannaMemoryArenaIdGfx };
        Utils::PopulateDeviceExtensions(enabledDeviceExtensions,
            m_PhysicalDevice,
            createInfo.m_LogicalDeviceCreateArgs.m_ppEnabledDeviceExtensions,
            createInfo.m_LogicalDeviceCreateArgs.m_EnabledDeviceExtensionCount);

        VkDeviceCreateInfo deviceCreateInfo {};
        Utils::PopulateVkDeviceCreateInfo(
            createInfo,
            queueCreateInfos.data(),
            queueCreateInfos.size(),
            deviceCreateInfo);

        VkResult result = vkCreateDevice(m_PhysicalDevice, &deviceCreateInfo, pAllocationCallbacks, &m_LogicalDevice);
        return result == VK_SUCCESS;
    }

    bool VkGpu::TryInitialize(
        const se_VkDriverCreateInfo_t &createInfo,
        VkInstance& instance,
        VkSurfaceKHR& surface,
        const VkAllocationCallbacks* pAllocationCallbacks)
    {
        if (!Utils::TrySelectPhysicalDevice(instance, m_PhysicalDevice, createInfo.m_PhysicalDeviceCreateArgs))
        {
            SAVANNA_LOG("Failed to select physical device.");
            return false;
        }

        vkGetPhysicalDeviceProperties(m_PhysicalDevice, &m_PhysicalDeviceProperties);

        if (!InitializeLogicalDevice(createInfo, instance, surface, pAllocationCallbacks))
        {
            SAVANNA_LOG("Failed to initialize logical device.");
            return false;
        }

        return true;
    }

    void VkGpu::Reset(
        VkInstance &instance,
        const VkAllocationCallbacks* pAllocationCallbacks)
    {
        if (m_LogicalDevice != VK_NULL_HANDLE)
        {
            vkDestroyDevice(m_LogicalDevice, pAllocationCallbacks);
            m_LogicalDevice = VK_NULL_HANDLE;
        }
        m_PhysicalDevice = VK_NULL_HANDLE;
        m_PhysicalDeviceProperties = {};
    }

    se_GfxErrorCode_t VkGpu::RequestSwapchain(
        const se_GfxSwapchainCreateInfo_t &createInfo,
        se_GfxHandle_t *const pOutSwapchainHandle)
    {
        return kSavannaGfxErrorCodeNotImplemented;
    }
} // namespace Savanna::Gfx::Vk2
