#pragma once

#include "SavannaVk.h"

#include <vulkan/vulkan.h>

#include "VkContext.h"
#include "Utilities/VkCallbacks.h"

namespace Savanna::Gfx::Vk
{
    class DebugMessenger
    {
    private:
        static constexpr const char* k_DefaultDebuggingExtensionName = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;

    public:
        static void PopulateDebugCreateInfo(
            VkDebugUtilsMessengerCreateInfoEXT* pDebugCreateInfo,
            void* pUserData = nullptr);

    private:
        VkInstance m_CreatorInstance;
        const VkAllocationCallbacks* m_AllocationCallbacks;
        VkDebugUtilsMessengerEXT m_DebugMessenger;

    public:
        DebugMessenger(
            VkInstance instance,
            VkDebugUtilsMessengerCreateInfoEXT* pDebugCreateInfo,
            const VkAllocationCallbacks* pAllocationCallbacks);

        ~DebugMessenger();
    };
} // namespace Savanna::Gfx::Vk
