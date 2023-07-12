#pragma once

#include <SavannaEngine.h>

#include <vulkan/vulkan.h>
#include "VkDefinitions.h"

namespace Savanna::Gfx::Vk
{
    static inline VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
        if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        {
            Console::Print("[SEVK] Validation Layer: {}", pCallbackData->pMessage);
        }

        return VK_FALSE;
    }
} // namespace Vulkan
