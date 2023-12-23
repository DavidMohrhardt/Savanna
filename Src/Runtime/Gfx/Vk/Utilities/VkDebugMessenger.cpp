#include "VkDebugMessenger.h"

namespace savanna::Gfx::Vk2::Utils
{
    static DebugMessenger* g_DebugMessenger = nullptr;

    static inline VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
        if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        {
            Console::Print("[SavannaVK2] DEBUG: {}", pCallbackData->pMessage);
        }

        return VK_FALSE;
    }

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

    void DebugMessenger::Initialize(
        VkInstance instance,
        const VkAllocationCallbacks* const pAllocationCallbacks)
    {
        if (g_DebugMessenger != nullptr)
        {
            return;
        }

        VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo {};
        debugUtilsMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugUtilsMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debugUtilsMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugUtilsMessengerCreateInfo.pfnUserCallback = DebugCallback;
        debugUtilsMessengerCreateInfo.pUserData = nullptr; // Optional

        g_DebugMessenger = ::new DebugMessenger(
            instance, &debugUtilsMessengerCreateInfo, pAllocationCallbacks);

        SAVANNA_DEBUG_LOG("Savanna Vulkan: Debug Messenger Initialized...");
    }

    void DebugMessenger::Destroy()
    {
        if (g_DebugMessenger != nullptr)
        {
            ::delete g_DebugMessenger;
            g_DebugMessenger = nullptr;
            SAVANNA_DEBUG_LOG("Savanna Vulkan: Debug Messenger Destroyed...");
        }
    }

    DebugMessenger::DebugMessenger(
        VkInstance instance,
        VkDebugUtilsMessengerCreateInfoEXT* pDebugCreateInfo,
        const VkAllocationCallbacks* pAllocationCallbacks)
        : m_CreatorInstance(instance)
        , m_AllocationCallbacks(pAllocationCallbacks)
    {
        auto result = CreateDebugUtilsMessengerEXT(m_CreatorInstance, pDebugCreateInfo, m_AllocationCallbacks, &m_DebugMessenger);
        if (result != VK_SUCCESS)
        {
            // TODO @davidmo refactor to not be a class constructor but a factory function
        }
    }

    DebugMessenger::~DebugMessenger()
    {
        PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_CreatorInstance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            func(m_CreatorInstance, m_DebugMessenger, m_AllocationCallbacks);
        }
    }
} // namespace savanna::Gfx::Vk
