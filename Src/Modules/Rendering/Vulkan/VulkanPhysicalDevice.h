#pragma once

// Standard Includes
#include <optional>

// Vulkan Includes
#include <vulkan/vulkan.h>

// Savanna Includes
#include <Memory/CacheLine.h>
#include <Types/Primitive/PrimitiveTypes.h>

// Module includes
#include "VulkanQueueFamilyDesc.h"

namespace Savanna::Rendering::Vulkan
{
    struct VulkanPhysicalDeviceDescriptor
    {
        VkPhysicalDevice physicalDevice;
        VkPhysicalDeviceProperties properties;
        VkPhysicalDeviceFeatures features;
        VkPhysicalDeviceMemoryProperties memoryProperties;
    };

    struct VulkanQueueFamilyIndices
    {
        std::optional<uint32> m_GraphicsQueueFamilyIndex;
        std::optional<uint32> m_ComputeQueueFamilyIndex;
        std::optional<uint32> m_TransferQueueFamilyIndex;
        std::optional<uint32> m_SparseBindingQueueFamilyIndex;

        bool HasGraphicsQueueFamilyIndex() const
        {
            return m_GraphicsQueueFamilyIndex.has_value();
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

        bool HasAllQueueFamilyIndices() const
        {
            return HasGraphicsQueueFamilyIndex()
                && HasComputeQueueFamilyIndex()
                && HasTransferQueueFamilyIndex()
                && HasSparseBindingQueueFamilyIndex();
        }
    };

    class VulkanPhysicalDevice
    {
    public:
        static uint32 GetPhysicalDeviceCount(const VkInstance& instance);

        static void GetPhysicalDeviceDescriptors(
            const VkInstance& instance,
            const uint32 count,
            VulkanPhysicalDeviceDescriptor* physicalDeviceDescriptorsPtr);

    private:
        VulkanPhysicalDeviceDescriptor m_Descriptor;
        VulkanQueueFamilyIndices m_QueueFamilyIndices;

    public:
        VulkanPhysicalDevice();
        VulkanPhysicalDevice(const VulkanPhysicalDeviceDescriptor& descriptor);

        VulkanPhysicalDevice(const VulkanPhysicalDevice& other) = default;
        VulkanPhysicalDevice(VulkanPhysicalDevice&& other) = default;

        ~VulkanPhysicalDevice() = default;

    public:
        VulkanPhysicalDevice& operator=(const VulkanPhysicalDevice& other) = default;
        VulkanPhysicalDevice& operator=(VulkanPhysicalDevice&& other) = default;

    public:
        VulkanGraphicsDevice CreateGraphicsDevice();

        const VulkanPhysicalDeviceDescriptor& GetDescriptor() const;
        const VkPhysicalDevice& GetPhysicalDevice() const;
        const VkPhysicalDeviceProperties& GetProperties() const;
        const VkPhysicalDeviceFeatures& GetFeatures() const;
        const VkPhysicalDeviceMemoryProperties& GetMemoryProperties() const;
        const VkPhysicalDeviceLimits& GetLimits() const;
        const VkPhysicalDeviceSparseProperties& GetSparseProperties() const;

        const VulkanQueueFamilyIndices& GetQueueFamilyIndices() const;
    };
} // namespace Savanna::Rendering::Vulkan
