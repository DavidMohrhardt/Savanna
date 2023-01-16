#include "VkDebugMessenger.h"

namespace Savanna::Gfx::Vk
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

    void DebugMessenger::PopulateDebugCreateInfo(
        VkDebugUtilsMessengerCreateInfoEXT* pDebugCreateInfo,
        void* pUserData)
    {
        pDebugCreateInfo->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        pDebugCreateInfo->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        pDebugCreateInfo->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        pDebugCreateInfo->pfnUserCallback = Savanna::Gfx::Vk::DebugCallback;
        pDebugCreateInfo->pUserData = pUserData; // Optional
    }

    DebugMessenger::DebugMessenger(
        VkInstance instance,
        VkDebugUtilsMessengerCreateInfoEXT* pDebugCreateInfo,
        const VkAllocationCallbacks* pAllocationCallbacks)
        : m_CreatorInstance(instance)
        , m_AllocationCallbacks(pAllocationCallbacks)
    {
        VK_CALL_OR_THROW(
            CreateDebugUtilsMessengerEXT(instance, pDebugCreateInfo, pAllocationCallbacks, &m_DebugMessenger),
            "Unable to create debug messenger.");
    }

    DebugMessenger::~DebugMessenger()
    {
        PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_CreatorInstance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            func(m_CreatorInstance, m_DebugMessenger, m_AllocationCallbacks);
        }
    }
} // namespace Savanna::Gfx::Vk
