#pragma once

#include "SavannaVk.h"

#include <vulkan/vulkan.h>

#include "VkRendererCreateInfo.h"
#include "Utilities/VkCallbacks.h"

namespace Savanna::Gfx::Vulkan
{
    class VkRendererDebugMessenger
    {
    // Constants
    private:
        static constexpr const char* k_DefaultDebuggingExtensionName = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;

    // Static Functions
    public:
        static void PopulateCreateInfo(
            VkDebugUtilsMessengerCreateInfoEXT* pDebugCreateInfo,
            void* pUserData = nullptr);

        // Members
    private:
        VkInstance m_CreatorInstance;
        const VkAllocationCallbacks* m_AllocationCallbacks;
        VkDebugUtilsMessengerEXT m_DebugMessenger;

    // Constructors/Destructors
    public:
        VkRendererDebugMessenger() = default;
        VkRendererDebugMessenger(
            VkInstance instance,
            VkDebugUtilsMessengerCreateInfoEXT *pDebugCreateInfo,
            const VkAllocationCallbacks *pAllocationCallbacks);

        ~VkRendererDebugMessenger();
    };
} // namespace Savanna::Gfx::Vulkan
