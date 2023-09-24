#include "VkInfoCreateUtils.h"

#include <set>

// #define SAVANNA_VK_APPLICATION_NAME "Savanna"
// #define SAVANNA_VK_ENGINE_NAME "Savanna Engine"
// #define SAVANNA_VK_API_VERSION VK_API_VERSION_1_3
// #define SAVANNA_VK_APPLICATION_VERSION VK_MAKE_VERSION(0, 1, 0, 0)

namespace Savanna::Gfx::Vk2::Utils
{
    // void GetUniqueVkQueueFamilies(
    //     VkDeviceQueueCreateInfo *pQueueCreateInfos,
    //     uint32& queueCreateInfoCount,
    //     const QueueFamilyIndices& queueFamilyIndices,
    //     float* pQueuePriority)
    // {
    //     std::set<uint32> uniqueQueueFamilies = {};

    //     if (queueFamilyIndices.HasGraphicsQueueFamilyIndex())
    //         uniqueQueueFamilies.insert(queueFamilyIndices.m_GraphicsQueueFamilyIndex.value());

    //     if (queueFamilyIndices.HasComputeQueueFamilyIndex())
    //         uniqueQueueFamilies.insert(queueFamilyIndices.m_ComputeQueueFamilyIndex.value());

    //     if (queueFamilyIndices.HasTransferQueueFamilyIndex())
    //         uniqueQueueFamilies.insert(queueFamilyIndices.m_TransferQueueFamilyIndex.value());

    //     if (queueFamilyIndices.HasSparseBindingQueueFamilyIndex())
    //         uniqueQueueFamilies.insert(queueFamilyIndices.m_SparseBindingQueueFamilyIndex.value());

    //     if (queueFamilyIndices.HasPresentQueueFamilyIndex())
    //         uniqueQueueFamilies.insert(queueFamilyIndices.m_PresentQueueFamilyIndex.value());

    //     queueCreateInfoCount = static_cast<uint32>(uniqueQueueFamilies.size());

    //     if (pQueueCreateInfos != nullptr)
    //     {
    //         uint32 count = 0;
    //         for (uint32 queueFamily : uniqueQueueFamilies)
    //         {
    //             VkDeviceQueueCreateInfo queueCreateInfo = {};
    //             queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    //             queueCreateInfo.queueFamilyIndex = queueFamily;
    //             queueCreateInfo.queueCount = 1;
    //             queueCreateInfo.pQueuePriorities = pQueuePriority;
    //             pQueueCreateInfos[count++] = queueCreateInfo;
    //         }
    //     }
    // }

    // void PopulateVkInstanceCreateInfo(
    //     const RendererCreateInfo& rendererCreateInfo,
    //     const VkApplicationInfo* pApplicationCreateInfo,
    //     VkInstanceCreateInfo& instanceCreateInfo)
    // {
    //     instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    //     instanceCreateInfo.pNext = nullptr;
    //     instanceCreateInfo.flags = 0;
    //     instanceCreateInfo.pApplicationInfo = pApplicationCreateInfo;
    //     instanceCreateInfo.enabledLayerCount = static_cast<uint32>(rendererCreateInfo.m_EnabledLayerCount);
    //     instanceCreateInfo.ppEnabledLayerNames = rendererCreateInfo.m_ppEnabledLayerNames;
    //     instanceCreateInfo.enabledExtensionCount = static_cast<uint32>(rendererCreateInfo.m_InstanceExtensionsCount);
    //     instanceCreateInfo.ppEnabledExtensionNames = rendererCreateInfo.m_ppInstanceExtensions;
    // }

    // void PopulateVkDeviceCreateInfo(
    //     const RendererCreateInfo& rendererCreateInfo,
    //     const VkDeviceQueueCreateInfo* pQueueCreateInfo,
    //     const uint32& queueCreateInfoCount,
    //     VkDeviceCreateInfo& deviceCreateInfo)
    // {
    //     deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    //     deviceCreateInfo.pNext = nullptr;
    //     deviceCreateInfo.flags = 0;
    //     deviceCreateInfo.queueCreateInfoCount = queueCreateInfoCount;
    //     deviceCreateInfo.pQueueCreateInfos = pQueueCreateInfo;
    //     deviceCreateInfo.enabledExtensionCount = static_cast<uint32>(rendererCreateInfo.m_DeviceExtensionsCount);
    //     deviceCreateInfo.ppEnabledExtensionNames = rendererCreateInfo.m_ppDeviceExtensions;
    //     // Device level layers are no longer relevant in Vulkan 1.1 and above.
    //     deviceCreateInfo.enabledLayerCount = 0;
    //     deviceCreateInfo.ppEnabledLayerNames = nullptr;

    //     deviceCreateInfo.pEnabledFeatures = nullptr;
    // }
} // namespace Savanna::Gfx::Vk::Utils
