#pragma once

#include "SavannaVulkan.h"

#include "Utilities/VulkanCallbacks.h"

namespace Savanna::Rendering::Vulkan
{
    class VulkanDebugMessenger
    {
    // Constants
    private:
        static constexpr const char* k_DefaultDebuggingExtensionName = "VK_EXT_debug_utils";

    // Static Members
    private:
    public:

    // Static Functions
    private:
    public:
        static void PopulateDebugCreateInfo(
            VulkanInstance* pVulkanInstance,
            VkDebugUtilsMessengerCreateInfoEXT* pDebugCreateInfo,
            void* pUserData);

    // Members
    private:
        VkInstance m_CreatorInstance;
        const VkAllocationCallbacks* m_AllocationCallbacks;
        VkDebugUtilsMessengerEXT m_DebugMessenger;

    public:

    // Constructors/Destructors
    private:
    public:
        VulkanDebugMessenger(
            VulkanInstance* pInstance,
            VkDebugUtilsMessengerCreateInfoEXT* pDebugCreateInfo,
            const VkAllocationCallbacks* pAllocationCallbacks);

        ~VulkanDebugMessenger();

    // Functions
    public:
    private:
    };
} // namespace Savanna::Rendering::Vulkan
