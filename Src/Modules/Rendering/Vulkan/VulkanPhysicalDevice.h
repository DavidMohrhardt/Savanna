#pragma once

// Vulkan Includes
#include <vulkan/vulkan.h>

// Savanna Includes
#include <Types/Primitive/PrimitiveTypes.h>

// Module includes
#include "VulkanQueueFamilyIndices.h"

namespace Savanna::Rendering::Vulkan
{
    struct VulkanPhysicalDeviceDescriptor
    {
        VkPhysicalDevice physicalDevice;
        VkPhysicalDeviceProperties properties;
        VkPhysicalDeviceFeatures features;
        VkPhysicalDeviceMemoryProperties memoryProperties;
    };

    class VulkanPhysicalDevice
    {
    public:
        static uint32 GetPhysicalDeviceCount(const VkInstance& instance);

        static void GetPhysicalDeviceDescriptors(
            const VkInstance& instance,
            const uint32 count,
            VulkanPhysicalDeviceDescriptor* physicalDeviceDescriptorsPtr);

        static void EnumeratePhysicalDevices(
            const VkInstance& instance,
            uint32& outCount,
            VulkanPhysicalDevice* physicalDevicesPtr = nullptr);

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
        const VulkanPhysicalDeviceDescriptor& GetDescriptor() const;
        const VkPhysicalDevice& GetPhysicalDevice() const;
        const VkPhysicalDeviceProperties& GetProperties() const;
        const VkPhysicalDeviceFeatures& GetFeatures() const;
        const VkPhysicalDeviceMemoryProperties& GetMemoryProperties() const;
        const VkPhysicalDeviceLimits& GetLimits() const;
        const VkPhysicalDeviceSparseProperties& GetSparseProperties() const;

        void ParseQueueFamilyIndices(VkSurfaceKHR* surfacePtr);
    };
} // namespace Savanna::Rendering::Vulkan
