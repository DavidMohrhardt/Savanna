#pragma once

// Savanna Includes
#include "SavannaVk2.h"

#include "Types/Containers/Arrays/dynamic_array.h"

#include <optional>

namespace Savanna::Gfx::Vk2
{
    struct VkQueueFamilyIndices
    {
    private:
        std::optional<uint32> m_GraphicsQueueFamilyIndex;
        std::optional<uint32> m_PresentQueueFamilyIndex;
        std::optional<uint32> m_ComputeQueueFamilyIndex;
        std::optional<uint32> m_TransferQueueFamilyIndex;
        std::optional<uint32> m_SparseBindingQueueFamilyIndex;

    public:
        VkQueueFamilyIndices(
            const VkPhysicalDevice physicalDevice,
            const VkSurfaceKHR surface = VK_NULL_HANDLE);

        VkQueueFamilyIndices() = default;
        VkQueueFamilyIndices(const VkQueueFamilyIndices& other) = default;
        VkQueueFamilyIndices(VkQueueFamilyIndices&& other) = default;
        VkQueueFamilyIndices& operator=(const VkQueueFamilyIndices& other) = default;
        VkQueueFamilyIndices& operator=(VkQueueFamilyIndices&& other) = default;
        ~VkQueueFamilyIndices() = default;

        void GetUniqueQueueFamilies(
            dynamic_array<VkDeviceQueueCreateInfo> &queueCreateInfos,
            float* pQueuePriority = nullptr);

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

        inline uint32 GetGraphicsQueueFamilyIndex() const { return m_GraphicsQueueFamilyIndex.value(); }
        inline uint32 GetPresentQueueFamilyIndex() const { return m_PresentQueueFamilyIndex.value(); }
        inline uint32 GetComputeQueueFamilyIndex() const { return m_ComputeQueueFamilyIndex.value(); }
        inline uint32 GetTransferQueueFamilyIndex() const { return m_TransferQueueFamilyIndex.value(); }
        inline uint32 GetSparseBindingQueueFamilyIndex() const { return m_SparseBindingQueueFamilyIndex.value(); }

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
