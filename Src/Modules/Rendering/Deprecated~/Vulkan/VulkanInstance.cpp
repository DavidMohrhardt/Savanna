#include "VulkanInstance.h"

#include "VulkanDebugMessenger.h"

#include <Profiling/Profiler.h>

namespace Savanna::Rendering::Vulkan
{
    VulkanInstance::Statics VulkanInstance::s_Statics;

    VulkanInstance::Statics::Statics()
    {
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        for (const auto& extension : extensions)
        {
            m_SupportedExtensions.insert(extension.extensionName);
        }

#if SAVANNA_DEBUG_LOGGING_ENABLED
        PrintAvailableExtensions();
#endif
    }

    void VulkanInstance::PrintAvailableExtensions()
    {
        std::string extensionsString = std::to_string(s_Statics.m_SupportedExtensions.size());
        extensionsString.append(" Total Available Extensions");
        for (const auto& extension : s_Statics.m_SupportedExtensions)
        {
            extensionsString.append("\n\t");
            extensionsString.append(extension);
        }

        SAVANNA_LOG("%s\n", extensionsString.c_str());
    }

    bool VulkanInstance::IsExtensionSupported(const char* extensionName)
    {
        return s_Statics.m_SupportedExtensions.contains(extensionName);
    }

    VulkanInstance::VulkanInstance()
        : m_VkInstance(VK_NULL_HANDLE)
        , m_DebugMessenger(nullptr)
        , m_EnableValidationLayers(false)
        , m_ActiveExtensions({})
    {
    }

    VulkanInstance::VulkanInstance(
        const FixedString32& applicationName,
        const FixedString32& engineName,
        const char** requiredExtensions,
        uint32_t requiredExtensionCount)
    {
        SAVANNA_INSERT_SCOPED_PROFILER("VulkanInstance::VulkanInstance ctor()");

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = applicationName;
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = engineName;
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo {};
        SetupValidationLayersIfNeeded(&createInfo, &debugCreateInfo, nullptr);

        bool allExtensionsSupported = true;
        if (requiredExtensionCount > 0)
        {
            for (int i = 0; i < requiredExtensionCount; i++)
            {
                allExtensionsSupported &= TryRequestExtension(requiredExtensions[i]);
            }

            createInfo.enabledExtensionCount = static_cast<uint32_t>(m_ActiveExtensions.size());
            createInfo.ppEnabledExtensionNames = m_ActiveExtensions.data();
        }
        else
        {
            createInfo.enabledExtensionCount = 0;
            createInfo.ppEnabledExtensionNames = nullptr;
        }

        if (!allExtensionsSupported)
        {
            throw Savanna::RuntimeErrorException("Not all required extensions are supported!");
        }

        VK_CALL_OR_THROW(
            vkCreateInstance(&createInfo, nullptr, &m_VkInstance),
            "Failed to create VkInstance!");

        OnPostInitialization(&debugCreateInfo, nullptr);
    }

    VulkanInstance::VulkanInstance(VkInstanceCreateInfo& createInfo)
    {
        SAVANNA_INSERT_SCOPED_PROFILER("VulkanInstance::VulkanInstance ctor(VkApplicationInfo&)");
        VK_CALL_OR_THROW(
            vkCreateInstance(&createInfo, nullptr, &m_VkInstance),
            "Failed to create VkInstance!");
    }

    VulkanInstance::VulkanInstance(VulkanInstance &&other)
    {
        *this = std::move(other);
    }

    VulkanInstance::~VulkanInstance()
    {
        m_DebugMessenger = nullptr;
        vkDestroyInstance(m_VkInstance, nullptr);
    }

    VulkanInstance& VulkanInstance::operator=(VulkanInstance &&other)
    {
        m_VkInstance = other.m_VkInstance;
        other.m_VkInstance = nullptr;

        m_DebugMessenger = std::move(other.m_DebugMessenger);
        m_ActiveExtensions = std::move(other.m_ActiveExtensions);

        return *this;
    }

    bool VulkanInstance::TryRequestExtension(const char* extensionName)
    {
        if (std::find(m_ActiveExtensions.begin(), m_ActiveExtensions.end(), extensionName) != m_ActiveExtensions.end())
        {
            return true;
        }

        if (IsExtensionSupported(extensionName))
        {
            m_ActiveExtensions.push_back(extensionName);
            return true;
        }

        SAVANNA_DEBUG_LOG("Extension %s is unsupported.", extensionName);

        return false;
    }

    bool VulkanInstance::CheckValidationLayerSupport()
    {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : k_DefaultValidationLayers)
        {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers)
            {
                if (strcmp(layerName, layerProperties.layerName) == 0)
                {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound)
            {
                return false;
            }
        }

        return true;
    }

    void VulkanInstance::SetupValidationLayersIfNeeded(
        VkInstanceCreateInfo* createInfo,
        VkDebugUtilsMessengerCreateInfoEXT* debugCreateInfo,
        void* userData)
    {
        // Validation Layers
        if (m_EnableValidationLayers)
        {
            if (!CheckValidationLayerSupport())
            {
                throw Savanna::RuntimeErrorException("validation layers requested, but are not supported.");
            }

            if (!TryRequestExtension(k_DefaultDebuggingExtensionName))
            {
                throw Savanna::RuntimeErrorException("Validation Layers requested but extension is unsupported.");
            }

            createInfo->enabledLayerCount = static_cast<uint32_t>(k_DefaultValidationLayers.size());
            createInfo->ppEnabledLayerNames = k_DefaultValidationLayers.data();
            VulkanDebugMessenger::PopulateDebugCreateInfo(this, debugCreateInfo, nullptr);
            createInfo->pNext = debugCreateInfo;
        }
        else
        {
            createInfo->enabledLayerCount = 0;
            createInfo->pNext = nullptr;
        }
    }

    void VulkanInstance::OnPostInitialization(
        VkDebugUtilsMessengerCreateInfoEXT* pDebugCreateInfo,
        const VkAllocationCallbacks* pAllocationCallbacks)
    {
        if (m_EnableValidationLayers)
        {
            m_DebugMessenger = std::unique_ptr<VulkanDebugMessenger>(
                new VulkanDebugMessenger(this, pDebugCreateInfo, pAllocationCallbacks));
        }
    }
} // namespace Savanna::Rendering::Vulkan
