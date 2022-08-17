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
        const std::vector<const char*> k_DefaultValidationLayers =
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
        inline static bool IsExtensionSupported(const char* extensionName) { return s_SupportedExtensions.contains(extensionName); }
        static void PrintAvailableExtensions();

    private: // Members
        VkInstance m_VulkanInstance;
        std::vector<const char*> m_RequestedExtensions;
        std::unique_ptr<VulkanDebugMessenger> m_DebugMessenger;

    public: // Constructors/Destructors
        VulkanInstance(const MemoryArena& memoryArena);
        ~VulkanInstance();

    public: // Functions
        bool TryCreateInstance(const FixedString32& applicationName, const FixedString32& engineName);

        bool TryRequestExtension(const char* extensionName);
        inline const std::vector<const char*> GetRequestedExtensions() const { return m_RequestedExtensions; }

        bool CheckValidationLayerSupport();
        // bool RequestValidationLayerEnabled(const char* layerName);

        const VkInstance GetInstance() const { return m_VulkanInstance; }

    private: // Functions
        void SetupValidationLayersIfRequested(
            VkInstanceCreateInfo* createInfo,
            VkDebugUtilsMessengerCreateInfoEXT* debugCreateInfo,
            void* userData);

        void AfterInstanceCreationValidationLayerSetup(
            VkDebugUtilsMessengerCreateInfoEXT* pDebugCreateInfo,
            const VkAllocationCallbacks* pAllocationCallbacks);
    };
} // namespace Savanna::Rendering::Vulkan
