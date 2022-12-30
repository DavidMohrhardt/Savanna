#include "VkRendererInitializationCache.h"

namespace Savanna::Gfx::Vk::Utils
{
    template <typename T>
    static void insert(std::vector<T>& container, const T* const items, const int itemCount)
    {
        for (int i = 0; i < itemCount; ++i)
        {
            container.push_back(items[i]);
        }
    }

    RendererInitializationCache::RendererInitializationCache(const RendererCreateInfo *const pCreateInfo)
        : m_CreateInfo(*pCreateInfo)
        , m_SupportedInstanceExtensions()
        , m_SupportedInstanceLayers()
        , m_RequestedInstanceExtensions()
        , m_RequestedInstanceLayers()
        , m_ValidationLayersEnabled(false)
    {
        ParseCreateInfo();
    }

    RendererInitializationCache::~RendererInitializationCache() {}

    bool RendererInitializationCache::IsInstanceExtensionSupported(const char *extensionName) const
    {
        return m_SupportedInstanceExtensions.find(extensionName) != m_SupportedInstanceExtensions.end();
    }

    bool RendererInitializationCache::IsInstanceLayerSupported(const char *layerName) const
    {
        return m_SupportedInstanceLayers.find(layerName) != m_SupportedInstanceLayers.end();
    }

    bool RendererInitializationCache::IsDeviceExtensionSupported(const char *extensionName) const
    {
        return false;
    }

    bool RendererInitializationCache::IsDeviceLayerSupported(const char *layerName) const
    {
        return false;
    }

    void RendererInitializationCache::ProcessInstanceExtensions()
    {
        PrepareSupportedInstanceExtensions();
        if (m_CreateInfo.m_InstanceExtensionsCount > 0 && m_CreateInfo.m_InstanceExtensions != nullptr)
        {
            insert(m_RequestedInstanceExtensions, m_CreateInfo.m_InstanceExtensions, m_CreateInfo.m_InstanceExtensionsCount);
        }

        if (m_ValidationLayersEnabled)
            insert(m_RequestedInstanceExtensions, &k_DebugUtilsExtensionName, 1);

        if (m_RequestsSurface)
            insert(m_RequestedInstanceExtensions, &k_SurfaceExtensionName, 1);

        if (m_RequestsSwapchain)
            insert(m_RequestedInstanceExtensions, &k_SwapchainExtensionName, 1);
    }

    void RendererInitializationCache::ProcessInstanceLayers()
    {
        if (m_ValidationLayersEnabled)
        {
            PrepareSupportedInstanceLayers();
            if (m_CreateInfo.m_EnabledLayerCount > 0 && m_CreateInfo.m_EnabledLayerNames != nullptr)
            {
                insert(m_RequestedInstanceLayers, m_CreateInfo.m_EnabledLayerNames, m_CreateInfo.m_EnabledLayerCount);
            }
            else
            {
                insert(m_RequestedInstanceLayers, &k_DefaultValidationLayerName, 1);
            }
        }
    }

    inline void RendererInitializationCache::ParseCreateFlags(const RendererCreateFlags &createFlags)
    {
        m_ValidationLayersEnabled = RendererCreateFlags::HasFlag(createFlags, SE_VK_ENABLE_VALIDATION_LAYERS_FLAG);
        m_RequestsSurface = RendererCreateFlags::HasFlag(createFlags, SE_VK_ENABLE_SURFACE_FLAG);
        m_RequestsSwapchain = RendererCreateFlags::HasFlag(createFlags, SE_VK_ENABLE_SWAPCHAIN_FLAG);
    }

    void RendererInitializationCache::ParseCreateInfo()
    {
        ParseCreateFlags(m_CreateInfo.m_CreateFlags);
        ProcessInstanceExtensions();
        ProcessInstanceLayers();
    }

    void RendererInitializationCache::PrepareSupportedInstanceExtensions()
    {
        uint32 extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
        for (const auto& extension : extensions)
        {
            m_SupportedInstanceExtensions.insert(extension.extensionName);
        }
    }

    void RendererInitializationCache::PrepareSupportedInstanceLayers()
    {
        uint32 layerCount = 0;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> layers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, layers.data());
        for (const auto& layer : layers)
        {
            m_SupportedInstanceLayers.insert(layer.layerName);
        }
    }
}
