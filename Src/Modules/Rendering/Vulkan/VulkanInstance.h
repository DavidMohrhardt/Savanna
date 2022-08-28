#pragma once

#include <SavannaEngine.h>
#include <Memory/MemoryArena.h>
#include <Types/Strings/FixedString.h>

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include <vulkan/vulkan.h>

#include "Utilities/VulkanCallbacks.h"
#include "Utilities/VulkanDefinitions.h"
#include "Utilities/VulkanMacros.h"
#include "Utilities/VulkanResultUtils.h"

#define USE_ALTERNATE_VK_DEBUG_UTILS 1

namespace Savanna::Rendering::Vulkan
{
    // Forward Declarations
    class VulkanDebugMessenger;

    class VulkanInstance
    {
    private: // Constants
        inline static const std::vector<const char*> k_DefaultValidationLayers =
        {
            "VK_LAYER_KHRONOS_validation"
        };

        const char* k_DefaultDebuggingExtensionName =
#if USE_ALTERNATE_VK_DEBUG_UTILS
            "VK_EXT_debug_utils";
#else
            "VK_EXT_DEBUG_UTILS_EXTENSION_NAME";
#endif

#ifndef SAVANNA_VULKAN_DEBUGGING
        bool m_EnableValidationLayers = false;
#else
        bool m_EnableValidationLayers = true;
#endif

    private: // Static Members
        static std::unordered_set<std::string> s_SupportedExtensions;
        static bool s_StaticsInitialized;

    private: // Static Functions
        inline static void InitializeStatics();
        inline static bool IsExtensionSupported(const char* extensionName);
        static void PrintAvailableExtensions();

    private: // Members
        VkInstance m_VkInstance;
        std::vector<const char*> m_ActiveExtensions;
        std::unique_ptr<VulkanDebugMessenger> m_DebugMessenger;

    private: // Private Constructors
        VulkanInstance(const VulkanInstance& other) = delete;

    public: // Constructors/Destructors
        VulkanInstance(
            const FixedString32& applicationName,
            const FixedString32& engineName,
            const char** requiredExtensions,
            uint32_t requiredExtensionCount);

        VulkanInstance(VkInstanceCreateInfo& createInfo);

        VulkanInstance();
        VulkanInstance(VulkanInstance&& other);

        ~VulkanInstance();

    public: // Operators
        VulkanInstance& operator=(const VulkanInstance& other) = delete;
        VulkanInstance& operator=(VulkanInstance&& other);

        operator VkInstance() const { return m_VkInstance; }

    public: // Functions
        inline VkInstance GetVkInstance() const { return m_VkInstance; }
        inline bool IsValid() const { return m_VkInstance != VK_NULL_HANDLE; }

        bool TryRequestExtension(const char* extensionName);
        inline const std::vector<const char*> GetActiveExtensions() const { return m_ActiveExtensions; }

        bool CheckValidationLayerSupport();
        // bool RequestValidationLayerEnabled(const char* layerName);

    private: // Functions
        void SetupValidationLayersIfNeeded(
            VkInstanceCreateInfo* createInfo,
            VkDebugUtilsMessengerCreateInfoEXT* debugCreateInfo,
            void* userData);

        void OnPostInitialization(
            VkDebugUtilsMessengerCreateInfoEXT* pDebugCreateInfo,
            const VkAllocationCallbacks* pAllocationCallbacks);
    };
} // namespace Savanna::Rendering::Vulkan
