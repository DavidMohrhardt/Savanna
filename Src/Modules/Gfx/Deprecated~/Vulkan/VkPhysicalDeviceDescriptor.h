#pragma once

#include "SavannaVk.h"

typedef struct se_VkPhysicalDeviceDescriptor_t
{
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
    VkPhysicalDeviceMemoryProperties memoryProperties;
} se_VkPhysicalDeviceDescriptor_t;
DECLARE_SAVANNA_EXTENDED_NAMESPACED_CPP_TYPE_DEF(Gfx::Vulkan, se_VkPhysicalDeviceDescriptor_t, VkPhysicalDeviceDescriptor);
