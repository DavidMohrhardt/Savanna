#include "VulkanDebugMessenger.h"

namespace Savanna::Rendering::Vulkan
{
    VkResult CreateDebugUtilsMessengerEXT(
        VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkDebugUtilsMessengerEXT* pDebugMessenger)
    {
        PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        }
        else
        {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void VulkanDebugMessenger::PopulateDebugCreateInfo(
        VulkanInstance* pVulkanInstance,
        VkDebugUtilsMessengerCreateInfoEXT* pDebugCreateInfo,
        void* pUserData)
    {
        if (pVulkanInstance == nullptr || !pVulkanInstance->TryRequestExtension(k_DefaultDebuggingExtensionName))
        {
            return;
        }

        pDebugCreateInfo->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        pDebugCreateInfo->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        pDebugCreateInfo->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        pDebugCreateInfo->pfnUserCallback = Savanna::Rendering::Vulkan::DebugCallback;
        pDebugCreateInfo->pUserData = pUserData; // Optional
    }

    VulkanDebugMessenger::VulkanDebugMessenger(
        VulkanInstance* pInstance,
        VkDebugUtilsMessengerCreateInfoEXT* pDebugCreateInfo,
        const VkAllocationCallbacks* pAllocationCallbacks)
        : m_CreatorInstance(pInstance->GetInstance())
        , m_AllocationCallbacks(pAllocationCallbacks)
    {
        VK_CALL_OR_THROW(
            CreateDebugUtilsMessengerEXT(pInstance->GetInstance(), pDebugCreateInfo, pAllocationCallbacks, &m_DebugMessenger),
            "Unable to create debug messenger.");
    }

    VulkanDebugMessenger::~VulkanDebugMessenger()
    {
        PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_CreatorInstance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            func(m_CreatorInstance, m_DebugMessenger, m_AllocationCallbacks);
        }
    }
} // namespace Savanna::Rendering::Vulkan
