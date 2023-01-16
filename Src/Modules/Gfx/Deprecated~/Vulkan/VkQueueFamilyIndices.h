#pragma once

#include "SavannaVk.h"

// Standard Includes
#include <optional>

// Vk Includes
#include <vulkan/vulkan.h>

namespace Savanna::Gfx::Vulkan
{
    struct VkQueueFamilyIndices
    {
    public:
        std::optional<uint32> m_GraphicsQueueFamilyIndex;
        std::optional<uint32> m_PresentQueueFamilyIndex;
        std::optional<uint32> m_ComputeQueueFamilyIndex;
        std::optional<uint32> m_TransferQueueFamilyIndex;
        std::optional<uint32> m_SparseBindingQueueFamilyIndex;

    public:
        VkQueueFamilyIndices(const VkPhysicalDevice& physicalDevice, VkSurfaceKHR* surfacePtr = nullptr);

        VkQueueFamilyIndices() = default;
        VkQueueFamilyIndices(const VkQueueFamilyIndices& other) = default;
        VkQueueFamilyIndices(VkQueueFamilyIndices&& other) = default;
        VkQueueFamilyIndices& operator=(const VkQueueFamilyIndices& other) = default;
        VkQueueFamilyIndices& operator=(VkQueueFamilyIndices&& other) = default;
        ~VkQueueFamilyIndices() = default;

    public:
        void ParseQueueFamilyIndices(const VkPhysicalDevice& physicalDevice, VkSurfaceKHR* surfacePtr = nullptr);

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
