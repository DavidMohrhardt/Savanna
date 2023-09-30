#pragma once

#include "SavannaVk.h"

typedef struct se_VkPhysicalDeviceDescriptor_t
{
    VkPhysicalDevice physicalDevice;
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
    VkPhysicalDeviceMemoryProperties memoryProperties;
} se_VkPhysicalDeviceDescriptor_t;

namespace Savanna::Gfx::Vk
{
    using PhysicalDeviceDescriptor = se_VkPhysicalDeviceDescriptor_t;
}
