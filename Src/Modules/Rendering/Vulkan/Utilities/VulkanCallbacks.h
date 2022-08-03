#pragma once

#include <SavannaEngine.h>

#include <vulkan/vulkan.h>
#include "VulkanDefinitions.h"

namespace Savanna::Rendering::Vulkan
{
    static inline VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
        if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        {
            SAVANNA_FATAL_LOG("Validation Layer: %s", pCallbackData->pMessage);
        }

        return VK_FALSE;
    }
} // namespace Vulkan
