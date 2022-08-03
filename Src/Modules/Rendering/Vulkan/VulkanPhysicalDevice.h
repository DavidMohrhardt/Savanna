#pragma once

#include <vulkan/vulkan.h>

#include <Memory/CacheLine.h>
#include <Types/Primitive/PrimitiveTypes.h>

#include "VulkanQueueFamilyDesc.h"

namespace Savanna::Rendering::Vulkan
{
    class VulkanPhysicalDevice // : IPhysicalGfxDevice
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
