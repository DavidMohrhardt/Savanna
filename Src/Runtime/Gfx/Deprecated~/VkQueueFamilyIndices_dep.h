#pragma once

// Savanna Includes
#include "SavannaVk.h"

// Vulkan Includes
#include <vulkan/vulkan.h>

#include <optional>

namespace Savanna::Gfx::Vk
{
    struct QueueFamilyIndices
    {
    public:
        std::optional<uint32> m_GraphicsQueueFamilyIndex;
        std::optional<uint32> m_PresentQueueFamilyIndex;
        std::optional<uint32> m_ComputeQueueFamilyIndex;
        std::optional<uint32> m_TransferQueueFamilyIndex;
        std::optional<uint32> m_SparseBindingQueueFamilyIndex;

    public:
        QueueFamilyIndices(const VkPhysicalDevice physicalDevice, const VkSurfaceKHR surface = VK_NULL_HANDLE);

        QueueFamilyIndices() = default;
        QueueFamilyIndices(const QueueFamilyIndices& other) = default;
        QueueFamilyIndices(QueueFamilyIndices&& other) = default;
        QueueFamilyIndices& operator=(const QueueFamilyIndices& other) = default;
        QueueFamilyIndices& operator=(QueueFamilyIndices&& other) = default;
        ~QueueFamilyIndices() = default;

    public:
        void SetupQueueFamilyCreateInfos(VkDeviceQueueCreateInfo* queueCreateInfos, const uint32 queueCreateInfoCount);
        void GetUniqueQueueFamilies(
            VkDeviceQueueCreateInfo *pQueueCreateInfos,
            uint32& queueCreateInfoCount,
            float* pQueuePriority = nullptr);

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
} // namespace Savanna::Gfx::Vk
