#pragma once

#include "SavannaVk.h"

typedef struct se_VkPhysicalDeviceDescriptor_t
{
    VkPhysicalDevice physicalDevice;
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
    VkPhysicalDeviceMemoryProperties memoryProperties;
} se_VkPhysicalDeviceDescriptor_t;
DECLARE_SAVANNA_EXTENDED_NAMESPACED_CPP_TYPE_DEF(Gfx::Vk, se_VkPhysicalDeviceDescriptor_t, PhysicalDeviceDescriptor);
