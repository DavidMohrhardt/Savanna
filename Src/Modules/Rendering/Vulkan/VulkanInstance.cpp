#include "VulkanInstance.h"

#include "VulkanDebugMessenger.h"

namespace Savanna::Rendering::Vulkan
{
    std::unordered_set<std::string> VulkanInstance::s_SupportedExtensions = {};

    bool VulkanInstance::s_StaticsInitialized = false;

    void VulkanInstance::PrintAvailableExtensions()
    {
        std::string extensionsString = std::to_string(s_SupportedExtensions.size());
        extensionsString.append(" Total Available Extensions");
        for (const auto& extension : s_SupportedExtensions)
        {
            extensionsString.append("\n\t");
            extensionsString.append(extension);
        }

        SAVANNA_LOG("%s\n", extensionsString.c_str());
    }

    void VulkanInstance::InitializeStatics()
    {
        if (!s_StaticsInitialized)
        {
            uint32_t extensionCount = 0;
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

            std::vector<VkExtensionProperties> extensions(extensionCount);
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

            for (const auto& extension : extensions)
            {
                s_SupportedExtensions.insert(extension.extensionName);
            }

#if SAVANNA_DEBUG_LOGGING_ENABLED
            PrintAvailableExtensions();
#endif

            s_StaticsInitialized = true;
        }
    }

    VulkanInstance::VulkanInstance()
    {
        InitializeStatics();
    }

    VulkanInstance::~VulkanInstance()
    {
        m_DebugMessenger = nullptr;
        vkDestroyInstance(m_VulkanInstance, nullptr);
    }

    bool VulkanInstance::TryCreateInstance(const FixedString32& applicationName, const FixedString32& engineName)
    {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = applicationName;
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo {};
        SetupValidationLayersIfRequested(&createInfo, &debugCreateInfo, nullptr);

        createInfo.enabledExtensionCount = static_cast<uint32_t>(m_RequestedExtensions.size());
        createInfo.ppEnabledExtensionNames = m_RequestedExtensions.data();

        VkResult result;
        VK_CALL(result, vkCreateInstance(&createInfo, nullptr, &m_VulkanInstance), "Failed to create VkInstance!");

        AfterInstanceCreationValidationLayerSetup(&debugCreateInfo, nullptr);

        return result == VK_SUCCESS;
    }

    bool VulkanInstance::TryRequestExtension(const char* extensionName)
    {
        if (IsExtensionSupported(extensionName))
        {
            m_RequestedExtensions.push_back(extensionName);
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

    void VulkanInstance::SetupValidationLayersIfRequested(
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

    void VulkanInstance::AfterInstanceCreationValidationLayerSetup(
        VkDebugUtilsMessengerCreateInfoEXT* pDebugCreateInfo,
        const VkAllocationCallbacks* pAllocationCallbacks)
    {
        if (m_EnableValidationLayers)
        {
            m_DebugMessenger = std::unique_ptr<VulkanDebugMessenger>(new VulkanDebugMessenger(this, pDebugCreateInfo, pAllocationCallbacks));
        }
    }
} // namespace Savanna::Rendering::Vulkan
