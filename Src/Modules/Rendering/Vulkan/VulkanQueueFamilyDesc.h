#pragma once

#include <Utilities/SavannaCoding.h>

/**
 * @brief
 */
typedef struct __se_vulkanQueueFamilyDesc
{
    __se_uint32 m_FamilyId;
} __se_vulkanQueueFamilyDesc;
DECLARE_SAVANNA_EXTENDED_NAMESPACED_CPP_TYPE_DEF(Rendering::Vulkan, __se_vulkanQueueFamilyDesc, VulkanQueueFamilyDesc);
