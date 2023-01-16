#include "VkQueueFamilyIndices.h"

#include <vector>

namespace Savanna::Gfx::Vulkan
{
    VkQueueFamilyIndices::VkQueueFamilyIndices(
        const VkPhysicalDevice& physicalDevice,
        VkSurfaceKHR* surfacePtr)
    {
        ParseQueueFamilyIndices(physicalDevice, surfacePtr);
    }

    void VkQueueFamilyIndices::ParseQueueFamilyIndices(
        const VkPhysicalDevice& physicalDevice,
        VkSurfaceKHR* surfacePtr)
    {
        if (HasAllQueueFamilyIndices())
        {
            return;
        }

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(
            physicalDevice, &queueFamilyCount, queueFamilyProperties.data());

        uint32 i = 0;
        for (const auto& properties : queueFamilyProperties)
        {
            VkQueueFlags flags = properties.queueFlags;

            if (!HasGraphicsQueueFamilyIndex() && flags & VK_QUEUE_GRAPHICS_BIT)
            {
                m_GraphicsQueueFamilyIndex = static_cast<uint32>(i);
            }

            if (!HasComputeQueueFamilyIndex() && flags & VK_QUEUE_COMPUTE_BIT)
            {
                m_ComputeQueueFamilyIndex = static_cast<uint32>(i);
            }

            if (!HasTransferQueueFamilyIndex() && flags & VK_QUEUE_TRANSFER_BIT)
            {
                m_TransferQueueFamilyIndex = static_cast<uint32>(i);
            }

            if (!HasSparseBindingQueueFamilyIndex() && flags & VK_QUEUE_SPARSE_BINDING_BIT)
            {
                m_SparseBindingQueueFamilyIndex = static_cast<uint32>(i);
            }

            if (surfacePtr != nullptr && !HasPresentQueueFamilyIndex())
            {
                VkBool32 presentSupport = false;
                vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, *surfacePtr, &presentSupport);
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
} // namespace Savanna::Gfx::Vulkan
