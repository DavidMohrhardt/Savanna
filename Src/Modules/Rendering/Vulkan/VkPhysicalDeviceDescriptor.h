#pragma once

#include "SavannaVk.h"

typedef struct seVkPhysicalDeviceDescriptor_t
{
    VkPhysicalDevice physicalDevice;
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
    VkPhysicalDeviceMemoryProperties memoryProperties;
} seVkPhysicalDeviceDescriptor_t;
DECLARE_SAVANNA_EXTENDED_NAMESPACED_CPP_TYPE_DEF(Gfx::Vk, seVkPhysicalDeviceDescriptor_t, PhysicalDeviceDescriptor);
