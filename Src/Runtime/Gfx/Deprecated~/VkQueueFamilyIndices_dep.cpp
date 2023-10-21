#include "VkVkQueueFamilyIndices.h"

#include "Utilities/SavannaCoding.h"

namespace Savanna::Gfx::Vk
{
    VkQueueFamilyIndices::VkQueueFamilyIndices(
        const VkPhysicalDevice physicalDevice,
        const VkSurfaceKHR surface)
    {
        PopulateVkQueueFamilyIndices(physicalDevice, surface);
    }

    void VkQueueFamilyIndices::PopulateVkQueueFamilyIndices(
        const VkPhysicalDevice& physicalDevice,
        const VkSurfaceKHR surface)
    {
        if (HasAllVkQueueFamilyIndices())
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

            if (surface != VK_NULL_HANDLE && !HasPresentQueueFamilyIndex())
            {
                VkBool32 presentSupport = false;
                vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
                if (presentSupport)
                {
                    m_PresentQueueFamilyIndex = static_cast<uint32>(i);
                }
            }

            if (HasAllVkQueueFamilyIndices())
            {
                return;
            }

            ++i;
        }
    }

} // namespace Savanna::Gfx::Vk
