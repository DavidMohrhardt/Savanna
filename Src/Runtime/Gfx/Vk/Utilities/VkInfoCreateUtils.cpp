#include "VkInfoCreateUtils.h"

#include "VkExtensionUtils.h"

namespace Savanna::Gfx::Vk2::Utils
{
    void PopulateInstanceCreateInfo(
        VkInstanceCreateInfo& outCreateInfo,
        const char** ppEnabledInstanceExtensions,
        uint32 enabledInstanceExtensionCount,
        const char** ppEnabledLayers,
        uint32 enabledLayerCount)
    {
        outCreateInfo = {};
        outCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        outCreateInfo.pNext = nullptr;
        outCreateInfo.flags = 0;
        outCreateInfo.pApplicationInfo = nullptr;
        outCreateInfo.enabledExtensionCount = enabledInstanceExtensionCount;
        outCreateInfo.ppEnabledExtensionNames = ppEnabledInstanceExtensions;
        outCreateInfo.enabledLayerCount = enabledLayerCount;
        outCreateInfo.ppEnabledLayerNames = ppEnabledLayers;
    }

    void PopulateVkDeviceCreateInfo(
        const se_VkDriverCreateInfo_t &createInfo,
        const VkDeviceQueueCreateInfo *pQueueCreateInfo,
        const uint32 queueCreateInfoCount,
        VkDeviceCreateInfo &deviceCreateInfo)
    {
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.pNext = nullptr;
        deviceCreateInfo.flags = 0;
        deviceCreateInfo.queueCreateInfoCount = queueCreateInfoCount;
        deviceCreateInfo.pQueueCreateInfos = pQueueCreateInfo;
        deviceCreateInfo.enabledExtensionCount = static_cast<uint32>(createInfo.m_LogicalDeviceCreateArgs.m_EnabledDeviceExtensionCount);
        deviceCreateInfo.ppEnabledExtensionNames = createInfo.m_LogicalDeviceCreateArgs.m_ppEnabledDeviceExtensions;
        // Device level layers are no longer relevant in Vulkan 1.1 and above.
        deviceCreateInfo.enabledLayerCount = 0;
        deviceCreateInfo.ppEnabledLayerNames = nullptr;

        deviceCreateInfo.pEnabledFeatures = nullptr;
    }
} // namespace Savanna::Gfx::Vk2::Utils
