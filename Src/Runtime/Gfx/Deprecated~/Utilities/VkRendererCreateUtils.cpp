#include "VkRendererCreateUtils.h"

#include <set>

namespace Savanna::Gfx::Vk::Utils
{
    void GetUniqueQueueFamilies(
        VkDeviceQueueCreateInfo *pQueueCreateInfos,
        uint32& queueCreateInfoCount,
        const QueueFamilyIndices& queueFamilyIndices,
        float* pQueuePriority)
    {
        std::set<uint32> uniqueQueueFamilies = {};

        if (queueFamilyIndices.HasGraphicsQueueFamilyIndex())
            uniqueQueueFamilies.insert(queueFamilyIndices.m_GraphicsQueueFamilyIndex.value());

        if (queueFamilyIndices.HasComputeQueueFamilyIndex())
            uniqueQueueFamilies.insert(queueFamilyIndices.m_ComputeQueueFamilyIndex.value());

        if (queueFamilyIndices.HasTransferQueueFamilyIndex())
            uniqueQueueFamilies.insert(queueFamilyIndices.m_TransferQueueFamilyIndex.value());

        if (queueFamilyIndices.HasSparseBindingQueueFamilyIndex())
            uniqueQueueFamilies.insert(queueFamilyIndices.m_SparseBindingQueueFamilyIndex.value());

        if (queueFamilyIndices.HasPresentQueueFamilyIndex())
            uniqueQueueFamilies.insert(queueFamilyIndices.m_PresentQueueFamilyIndex.value());

        queueCreateInfoCount = static_cast<uint32>(uniqueQueueFamilies.size());

        if (pQueueCreateInfos != nullptr)
        {
            uint32 count = 0;
            for (uint32 queueFamily : uniqueQueueFamilies)
            {
                VkDeviceQueueCreateInfo queueCreateInfo = {};
                queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                queueCreateInfo.queueFamilyIndex = queueFamily;
                queueCreateInfo.queueCount = 1;
                queueCreateInfo.pQueuePriorities = pQueuePriority;
                pQueueCreateInfos[count++] = queueCreateInfo;
            }
        }
    }

    void PopulateVkApplicationInfo(
        const RendererCreateInfo& rendererCreateInfo,
        VkApplicationInfo &applicationInfo)
    {
        applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInfo.pApplicationName = rendererCreateInfo.m_ApplicationName;
        applicationInfo.applicationVersion = VK_MAKE_API_VERSION(
            rendererCreateInfo.m_ApplicationVersion.m_Variant,
            rendererCreateInfo.m_ApplicationVersion.m_Major,
            rendererCreateInfo.m_ApplicationVersion.m_Minor,
            rendererCreateInfo.m_ApplicationVersion.m_Patch);

        applicationInfo.pEngineName = rendererCreateInfo.m_EngineName;
        applicationInfo.applicationVersion = VK_MAKE_API_VERSION(
            rendererCreateInfo.m_EngineVersion.m_Variant,
            rendererCreateInfo.m_EngineVersion.m_Major,
            rendererCreateInfo.m_EngineVersion.m_Minor,
            rendererCreateInfo.m_EngineVersion.m_Patch);

        applicationInfo.apiVersion = VK_API_VERSION_1_3;
        applicationInfo.pNext = nullptr;
    }

    void PopulateVkInstanceCreateInfo(
        const RendererCreateInfo& rendererCreateInfo,
        const VkApplicationInfo* pApplicationCreateInfo,
        VkInstanceCreateInfo& instanceCreateInfo)
    {
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pNext = nullptr;
        instanceCreateInfo.flags = 0;
        instanceCreateInfo.pApplicationInfo = pApplicationCreateInfo;
        instanceCreateInfo.enabledLayerCount = static_cast<uint32>(rendererCreateInfo.m_EnabledLayerCount);
        instanceCreateInfo.ppEnabledLayerNames = rendererCreateInfo.m_ppEnabledLayerNames;
        instanceCreateInfo.enabledExtensionCount = static_cast<uint32>(rendererCreateInfo.m_InstanceExtensionsCount);
        instanceCreateInfo.ppEnabledExtensionNames = rendererCreateInfo.m_ppInstanceExtensions;
    }

    void PopulateVkDeviceCreateInfo(
        const RendererCreateInfo& rendererCreateInfo,
        const VkDeviceQueueCreateInfo* pQueueCreateInfo,
        const uint32& queueCreateInfoCount,
        VkDeviceCreateInfo& deviceCreateInfo)
    {
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.pNext = nullptr;
        deviceCreateInfo.flags = 0;
        deviceCreateInfo.queueCreateInfoCount = queueCreateInfoCount;
        deviceCreateInfo.pQueueCreateInfos = pQueueCreateInfo;
        deviceCreateInfo.enabledExtensionCount = static_cast<uint32>(rendererCreateInfo.m_DeviceExtensionsCount);
        deviceCreateInfo.ppEnabledExtensionNames = rendererCreateInfo.m_ppDeviceExtensions;
        // Device level layers are no longer relevant in Vulkan 1.1 and above.
        deviceCreateInfo.enabledLayerCount = 0;
        deviceCreateInfo.ppEnabledLayerNames = nullptr;

        deviceCreateInfo.pEnabledFeatures = nullptr;
    }
} // namespace Savanna::Gfx::Vk::Utils
