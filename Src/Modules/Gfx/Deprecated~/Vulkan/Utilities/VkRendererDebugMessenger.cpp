#include "VkRendererDebugMessenger.h"

namespace Savanna::Gfx::Vulkan
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

    void VkRendererDebugMessenger::PopulateCreateInfo(
        VkDebugUtilsMessengerCreateInfoEXT* pDebugCreateInfo,
        void* pUserData)
    {
        pDebugCreateInfo->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        pDebugCreateInfo->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        pDebugCreateInfo->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        pDebugCreateInfo->pfnUserCallback = Savanna::Gfx::Vulkan::DebugCallback;
        pDebugCreateInfo->pUserData = pUserData; // Optional
    }

    VkRendererDebugMessenger::VkRendererDebugMessenger(
        VkInstance instance,
        VkDebugUtilsMessengerCreateInfoEXT* pDebugCreateInfo,
        const VkAllocationCallbacks* pAllocationCallbacks)
        : m_CreatorInstance(instance)
        , m_AllocationCallbacks(pAllocationCallbacks)
    {
        if (CreateDebugUtilsMessengerEXT(instance, pDebugCreateInfo, pAllocationCallbacks, &m_DebugMessenger) != VK_SUCCESS)
        {
            throw std::runtime_error("Unable to create debug messenger.");
        }
    }

    VkRendererDebugMessenger::~VkRendererDebugMessenger()
    {
        PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_CreatorInstance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            func(m_CreatorInstance, m_DebugMessenger, m_AllocationCallbacks);
        }
    }
} // namespace Savanna::Gfx::Vulkan
