#pragma once

#include <vulkan/vulkan.h>

#include <Memory/CacheLine.h>
#include <Types/Primitive/PrimitiveTypes.h>

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

    class VulkanPhysicalDevice
    {
    private:
        static uint32 s_PhysicalDeviceCount;
    private:
        VkPhysicalDevice m_VulkanPhysicalDevice;
    public:
        VulkanPhysicalDevice(VkInstance instance);
        ~VulkanPhysicalDevice();

    public:
    };
} // namespace Savanna::Rendering::Vulkan
