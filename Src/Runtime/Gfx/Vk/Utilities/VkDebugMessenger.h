#pragma once

#include "SavannaVk2.h"

#include <vulkan/vulkan.h>

namespace Savanna::Gfx::Vk2::Utils
{
    class DebugMessenger
    {
    private:
        static constexpr const char* k_DefaultDebuggingExtensionName = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;

    public:
        static void Initialize(
            VkInstance instance,
            VkAllocationCallbacks* pAllocationCallbacks = nullptr);

        static void Destroy();

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
