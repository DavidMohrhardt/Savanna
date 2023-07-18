#pragma once

// Core Engine Includes
#include "SavannaEngine.h"

// Vulkan Includes
#include <vulkan/vulkan.h>

// Savanna Vulkan Includes
#include "Utilities/VkDefinitions.h"

#if __cplusplus
namespace Savanna::Gfx::Vulkan
{
#if SAVANNA_VK_DYNAMIC_INCLUDE
    VkGetInstanceProcAddrFunc s_GetInstanceProcAddr = nullptr;
#endif // SAVANNA_VK_DYNAMIC_INCLUDE
} // namespace Savanna::Gfx::Vulkan
#endif // end __cplusplus
