#pragma once

// Standard Includes
#include <optional>

// Vulkan Includes
#include <vulkan/vulkan.h>

// Savanna Includes
#include <Types/Primitive/PrimitiveTypes.h>

namespace Savanna::Rendering::Vulkan
{
    struct VulkanQueueFamilyIndices
    {
    public:
        std::optional<uint32> m_GraphicsQueueFamilyIndex;
        std::optional<uint32> m_PresentQueueFamilyIndex;
        std::optional<uint32> m_ComputeQueueFamilyIndex;
        std::optional<uint32> m_TransferQueueFamilyIndex;
        std::optional<uint32> m_SparseBindingQueueFamilyIndex;

    public:
        VulkanQueueFamilyIndices(const VkPhysicalDevice& physicalDevice, VkSurfaceKHR* surfacePtr = nullptr);

        VulkanQueueFamilyIndices() = default;
        VulkanQueueFamilyIndices(const VulkanQueueFamilyIndices& other) = default;
        VulkanQueueFamilyIndices(VulkanQueueFamilyIndices&& other) = default;
        VulkanQueueFamilyIndices& operator=(const VulkanQueueFamilyIndices& other) = default;
        VulkanQueueFamilyIndices& operator=(VulkanQueueFamilyIndices&& other) = default;
        ~VulkanQueueFamilyIndices() = default;

    public:
        void ParseQueueFamilyIndices(const VkPhysicalDevice& physicalDevice, VkSurfaceKHR* surfacePtr = nullptr);
        void SetupQueueFamilyCreateInfos(VkDeviceQueueCreateInfo* queueCreateInfos, const uint32 queueCreateInfoCount);

    public:
        bool HasGraphicsQueueFamilyIndex() const
        {
            return m_GraphicsQueueFamilyIndex.has_value();
        }

        bool HasPresentQueueFamilyIndex() const
        {
            return m_PresentQueueFamilyIndex.has_value();
        }

        bool HasComputeQueueFamilyIndex() const
        {
            return m_ComputeQueueFamilyIndex.has_value();
        }

        bool HasTransferQueueFamilyIndex() const
        {
            return m_TransferQueueFamilyIndex.has_value();
        }

        bool HasSparseBindingQueueFamilyIndex() const
        {
            return m_SparseBindingQueueFamilyIndex.has_value();
        }

        uint8 GetQueueFamilyCount() const
        {
            return static_cast<uint8>(
                HasGraphicsQueueFamilyIndex() +
                HasPresentQueueFamilyIndex() +
                HasComputeQueueFamilyIndex() +
                HasTransferQueueFamilyIndex() +
                HasSparseBindingQueueFamilyIndex());
        }

        bool HasAllQueueFamilyIndices() const
        {
            return HasGraphicsQueueFamilyIndex()
                && HasPresentQueueFamilyIndex()
                && HasComputeQueueFamilyIndex()
                && HasTransferQueueFamilyIndex()
                && HasSparseBindingQueueFamilyIndex();
        }

        bool HasAnyQueueFamilyIndices() const
        {
            return HasGraphicsQueueFamilyIndex()
                || HasPresentQueueFamilyIndex()
                || HasComputeQueueFamilyIndex()
                || HasTransferQueueFamilyIndex()
                || HasSparseBindingQueueFamilyIndex();
        }
    };
} // namespace Savanna::Rendering::Vulkan
