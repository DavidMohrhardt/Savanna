#pragma once

#include "SavannaVk.h"

#include <vulkan/vulkan.h>

#include "VkContext.h"
#include "Utilities/VkCallbacks.h"

namespace Savanna::Gfx::Vk
{
    class DebugMessenger
    {
    // Constants
    private:
        static constexpr const char* k_DefaultDebuggingExtensionName = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;

    // Static Members
    private:
    public:

    // Static Functions
    private:
    public:
        static void PopulateDebugCreateInfo(
            VkDebugUtilsMessengerCreateInfoEXT* pDebugCreateInfo,
            void* pUserData = nullptr);

    // Members
    private:
        VkInstance m_CreatorInstance;
        const VkAllocationCallbacks* m_AllocationCallbacks;
        VkDebugUtilsMessengerEXT m_DebugMessenger;

    public:

    // Constructors/Destructors
    private:
    public:
        DebugMessenger(
            VkInstance instance,
            VkDebugUtilsMessengerCreateInfoEXT* pDebugCreateInfo,
            const VkAllocationCallbacks* pAllocationCallbacks);

        ~DebugMessenger();

    // Functions
    public:
    private:
    };
} // namespace Savanna::Gfx::Vk
