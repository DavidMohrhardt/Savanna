#include "VkQueueFamilyIndices.h"

#include "Memory/MemoryManager.h"
#include "Types/Containers/Arrays/dynamic_array.h"

#include <set>

namespace Savanna::Gfx::Vk2
{
    VkQueueFamilyIndices::VkQueueFamilyIndices(
        const VkPhysicalDevice physicalDevice,
        const VkSurfaceKHR surface)
    {
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
        dynamic_array<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount, kSavannaAllocatorKindTemp);
        queueFamilyProperties.resize(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(
            physicalDevice, &queueFamilyCount, queueFamilyProperties.data());

        uint32 i = 0;
        for (const auto& properties : queueFamilyProperties)
        {
            VkQueueFlags flags = properties.queueFlags;

            if (!HasGraphicsQueueFamilyIndex() && flags.HasFlag(VK_QUEUE_GRAPHICS_BIT))
            {
                m_GraphicsQueueFamilyIndex = static_cast<uint32>(i);
            }

            if (!HasComputeQueueFamilyIndex() && flags.HasFlag(VK_QUEUE_COMPUTE_BIT))
            {
                m_ComputeQueueFamilyIndex = static_cast<uint32>(i);
            }

            if (!HasTransferQueueFamilyIndex() && flags.HasFlag(VK_QUEUE_TRANSFER_BIT))
            {
                m_TransferQueueFamilyIndex = static_cast<uint32>(i);
            }

            if (!HasSparseBindingQueueFamilyIndex() && flags.HasFlag(VK_QUEUE_SPARSE_BINDING_BIT))
            {
                m_SparseBindingQueueFamilyIndex = static_cast<uint32>(i);
            }

            if (surface != VK_NULL_HANDLE && !HasPresentQueueFamilyIndex())
            {
                VkBool32 presentSupport = false;
                vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
                if (presentSupport)
                {
                    m_PresentQueueFamilyIndex = static_cast<uint32>(i);
                }
            }

            if (HasAllQueueFamilyIndices())
            {
                return;
            }

            ++i;
        }
    }

    void VkQueueFamilyIndices::GetUniqueQueueFamilies(
        dynamic_array<VkDeviceQueueCreateInfo>& queueCreateInfos,
        float *pQueuePriority)
    {
        std::set<uint32> uniqueQueueFamilies = {};

        if (HasGraphicsQueueFamilyIndex())
            uniqueQueueFamilies.insert(m_GraphicsQueueFamilyIndex.value());

        if (HasComputeQueueFamilyIndex())
            uniqueQueueFamilies.insert(m_ComputeQueueFamilyIndex.value());

        if (HasTransferQueueFamilyIndex())
            uniqueQueueFamilies.insert(m_TransferQueueFamilyIndex.value());

        if (HasSparseBindingQueueFamilyIndex())
            uniqueQueueFamilies.insert(m_SparseBindingQueueFamilyIndex.value());

        if (HasPresentQueueFamilyIndex())
            uniqueQueueFamilies.insert(m_PresentQueueFamilyIndex.value());

        uint32 queueCreateInfoCount = static_cast<uint32>(uniqueQueueFamilies.size());

        queueCreateInfos.clear();
        queueCreateInfos.resize(queueCreateInfoCount);

        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        int32 index = 0;
        for (uint32 queueFamily : uniqueQueueFamilies)
        {
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = pQueuePriority;
            queueCreateInfos[index++] = queueCreateInfo;
        }
    }
} // namespace Savanna::Gfx::Vk
