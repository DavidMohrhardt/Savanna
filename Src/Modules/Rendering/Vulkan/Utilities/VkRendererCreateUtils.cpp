#include "VkRendererCreateUtils.h"

#include <set>

namespace Savanna::Gfx::Vk::Utils
{
    void GetUniqueQueueFamilies(
        VkDeviceQueueCreateInfo *pQueueCreateInfos,
        uint32 *pQueueCreateInfoCount,
        const RendererCreateInfo *const pRendererCreateInfo,
        const QueueFamilyIndices &queueFamilyIndices,
        float* pQueuePriority)
    {
        if (pQueueCreateInfoCount == nullptr)
        {
            throw Savanna::RuntimeErrorException("pQueueCreateInfoCount is null");
        }

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

        *pQueueCreateInfoCount = static_cast<uint32>(uniqueQueueFamilies.size());

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

    void PopulateVkApplicationInfo(VkApplicationInfo &applicationInfo, const RendererCreateInfo *const pRendererCreateInfo)
    {
        applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInfo.pNext = nullptr;
        applicationInfo.pApplicationName = pRendererCreateInfo->m_ApplicationName;
        applicationInfo.applicationVersion = VK_MAKE_API_VERSION(
            pRendererCreateInfo->m_ApplicationVersion.variant,
            pRendererCreateInfo->m_ApplicationVersion.major,
            pRendererCreateInfo->m_ApplicationVersion.minor,
            pRendererCreateInfo->m_ApplicationVersion.patch);

        applicationInfo.pEngineName = pRendererCreateInfo->m_EngineName;
        applicationInfo.applicationVersion = VK_MAKE_API_VERSION(
            pRendererCreateInfo->m_EngineVersion.variant,
            pRendererCreateInfo->m_EngineVersion.major,
            pRendererCreateInfo->m_EngineVersion.minor,
            pRendererCreateInfo->m_EngineVersion.patch);

        applicationInfo.apiVersion = VK_API_VERSION_1_0;
    }

    void PopulateVkInstanceCreateInfo(
        VkInstanceCreateInfo* pInstanceCreateInfo,
        const RendererCreateInfo* const pRendererCreateInfo,
        VkApplicationInfo* pApplicationCreateInfo)
    {
        if (pRendererCreateInfo == nullptr)
        {
            throw Savanna::RuntimeErrorException("pRendererCreateInfo is null");
        }

        VkApplicationInfo applicationCreateInfo = {};
        if (pApplicationCreateInfo != nullptr)
        {
            applicationCreateInfo = *pApplicationCreateInfo;
        }
        else
        {
            PopulateVkApplicationInfo(applicationCreateInfo, pRendererCreateInfo);
        }

        pInstanceCreateInfo->sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        pInstanceCreateInfo->pNext = nullptr;
        pInstanceCreateInfo->flags = 0;
        pInstanceCreateInfo->pApplicationInfo = &applicationCreateInfo;
        pInstanceCreateInfo->enabledLayerCount = static_cast<uint32>(pRendererCreateInfo->m_EnabledLayerCount);
        pInstanceCreateInfo->ppEnabledLayerNames = pRendererCreateInfo->m_ppEnabledLayerNames;
        pInstanceCreateInfo->enabledExtensionCount = static_cast<uint32>(pRendererCreateInfo->m_InstanceExtensionsCount);
        pInstanceCreateInfo->ppEnabledExtensionNames = pRendererCreateInfo->m_ppInstanceExtensions;
    }

    void PopulateVkDeviceCreateInfo(
        VkDeviceCreateInfo* pDeviceCreateInfo,
        const RendererCreateInfo* const pRendererCreateInfo,
        const VkDeviceQueueCreateInfo* pQueueCreateInfos,
        uint32 queueCreateInfoCount)
    {
        if (pDeviceCreateInfo == nullptr || pRendererCreateInfo == nullptr || pQueueCreateInfos == nullptr)
        {
            throw Savanna::RuntimeErrorException("pDeviceCreateInfo, pRendererCreateInfo, or pQueueCreateInfos is null");
        }
        pDeviceCreateInfo->sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        pDeviceCreateInfo->pNext = nullptr;
        pDeviceCreateInfo->flags = 0;
        pDeviceCreateInfo->queueCreateInfoCount = queueCreateInfoCount;
        pDeviceCreateInfo->pQueueCreateInfos = pQueueCreateInfos;
        pDeviceCreateInfo->enabledExtensionCount = static_cast<uint32>(pRendererCreateInfo->m_DeviceExtensionsCount);
        pDeviceCreateInfo->ppEnabledExtensionNames = pRendererCreateInfo->m_ppDeviceExtensions;
        pDeviceCreateInfo->enabledLayerCount = static_cast<uint32>(pRendererCreateInfo->m_EnabledLayerCount);
        pDeviceCreateInfo->ppEnabledLayerNames = pRendererCreateInfo->m_ppEnabledLayerNames;
        pDeviceCreateInfo->pEnabledFeatures = nullptr;
    }

} // namespace Savanna::Gfx::Vk::Utils
