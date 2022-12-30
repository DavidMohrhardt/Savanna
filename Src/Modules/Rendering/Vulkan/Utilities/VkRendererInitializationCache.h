#pragma once

#include <SavannaEngine.h>

#include <Utilities/SavannaCoding.h>
#include <Types/Singleton/Singleton.h>
#include <Types/Strings/FixedString.h>

#include "VkRendererCreateInfo.h"
#include "VkQueueFamilyIndices.h"

#include <vulkan/vulkan.h>

#include <string>
#include <unordered_set>
#include <vector>

namespace Savanna::Gfx::Vk::Utils
{
    class RendererInitializationCache : public Singleton<RendererInitializationCache>
    {
    private:
        const char* k_DefaultValidationLayerName = "VK_LAYER_KHRONOS_validation";
        const char* k_DebugUtilsExtensionName = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
        const char* k_SurfaceExtensionName = VK_KHR_SURFACE_EXTENSION_NAME;
        const char* k_SwapchainExtensionName = VK_KHR_SWAPCHAIN_EXTENSION_NAME;

    public:
        RendererInitializationCache(const RendererCreateInfo* const pCreateInfo);
        ~RendererInitializationCache();

    public:
        bool IsInstanceExtensionSupported(const char* extensionName) const;
        bool IsInstanceLayerSupported(const char* layerName) const;
        bool IsDeviceExtensionSupported(const char* extensionName) const;
        bool IsDeviceLayerSupported(const char* layerName) const;

    // maybe make this private with friend classes
        void SetVkInstance(VkInstance instance) { m_Instance = instance; }
        VkInstance GetVkInstance() const { return m_Instance; }

        void SetVkSurface(VkSurfaceKHR surface) { m_Surface = surface; }
        VkSurfaceKHR GetVkSurface() const { return m_Surface; }

        void SetVkPhysicalDevice(VkPhysicalDevice physicalDevice) { m_PhysicalDevice = physicalDevice; }
        VkPhysicalDevice GetVkPhysicalDevice() const { return m_PhysicalDevice; }

        void SetVkDevice(VkDevice device) { m_Device = device; }
        VkDevice GetVkDevice() const { return m_Device; }

        void SetQueueFamilyIndices(const QueueFamilyIndices& queueFamilyIndices) { m_QueueFamilyIndices = queueFamilyIndices; }
        const QueueFamilyIndices& GetQueueFamilyIndices() const { return m_QueueFamilyIndices; }

        size_t GetEnabledInstanceExtensionCount() const { return m_RequestedInstanceExtensions.size(); }
        const auto GetEnabledInstanceExtensionNames() const { return m_RequestedInstanceExtensions.data(); }

        size_t GetEnabledInstanceLayerCount() const { return m_RequestedInstanceLayers.size(); }
        const auto GetEnabledInstanceLayerNames() const { return m_RequestedInstanceLayers.data(); }

    private:
        void ProcessInstanceExtensions();
        void ProcessInstanceLayers();

        void ParseCreateFlags(const RendererCreateFlags& createFlags);
        void ParseCreateInfo();

        void PrepareSupportedInstanceExtensions();
        void PrepareSupportedInstanceLayers();
        void PrepareSupportedDeviceExtensions();
        void PrepareSupportedDeviceLayers();

    private:
        RendererCreateInfo m_CreateInfo;

        VkInstance m_Instance = VK_NULL_HANDLE;
        VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
        VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
        VkDevice m_Device = VK_NULL_HANDLE;

        QueueFamilyIndices m_QueueFamilyIndices;

        std::unordered_set<std::string> m_SupportedInstanceExtensions;
        std::unordered_set<std::string> m_SupportedInstanceLayers;
        std::unordered_set<std::string> m_SupportedDeviceExtensions;
        std::unordered_set<std::string> m_SupportedDeviceLayers;

        std::vector<const char*> m_RequestedInstanceExtensions;
        std::vector<const char*> m_RequestedInstanceLayers;

        bool m_ValidationLayersEnabled;
        bool m_RequestsSurface;
        bool m_RequestsSwapchain;
    };
} // namespace Savanna::Gfx::Vk
