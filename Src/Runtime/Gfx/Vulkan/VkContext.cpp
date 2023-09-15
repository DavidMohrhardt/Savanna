                                                   #include "VkContext.h"

#include "VkDebugMessenger.h"
#include "VkRendererCreateInfo.h"

#include "Utilities/VkExtensionUtils.h"
#include "Utilities/VkRendererCreateUtils.h"

#include <Profiling/Profiler.h>

namespace Savanna::Gfx::Vk
{
    Context::Context()
        : m_Instance(VK_NULL_HANDLE)
        , m_DebugMessenger(nullptr)
    {
    }

    Context::Context(Context &&other)
    {
        *this = std::move(other);
    }

    Context::Context(const RendererCreateInfo* const pRendererCreateInfo)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(Context::Context(const RendererCreateInfo *const pRendererCreateInfo));

        VkApplicationInfo applicationCreateInfo{};
        Utils::PopulateVkApplicationInfo(*pRendererCreateInfo, applicationCreateInfo);

        VkInstanceCreateInfo instanceCreateInfo{};
        Utils::PopulateVkInstanceCreateInfo(*pRendererCreateInfo, &applicationCreateInfo, instanceCreateInfo);

        CreateVkInstance(&instanceCreateInfo);
        if (instanceCreateInfo.enabledLayerCount > 0)
            CreateDebugMessenger(pRendererCreateInfo);
    }

    Context::~Context()
    {
        m_DebugMessenger = nullptr;

        if (m_Instance != VK_NULL_HANDLE)
            vkDestroyInstance(m_Instance, nullptr);
    }

    Context &Context::operator=(Context &&other)
    {
        if (this != &other)
        {
            m_Instance = other.m_Instance;
            m_DebugMessenger = std::move(other.m_DebugMessenger);
            other.m_Instance = VK_NULL_HANDLE;
        }
        return *this;
    }

    inline void Context::CreateVkInstance(VkInstanceCreateInfo* pInstanceCreateInfo)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(Context::CreateVkInstance(VkInstanceCreateInfo *pInstanceCreateInfo));

#if SAVANNA_VULKAN_VALIDATION_LAYERS
        Utils::ValidateInstanceExtensions(pInstanceCreateInfo->ppEnabledExtensionNames, pInstanceCreateInfo->enabledExtensionCount);
#endif

        if (vkCreateInstance(pInstanceCreateInfo, nullptr, &m_Instance) != VK_SUCCESS)
        {
            throw Savanna::RuntimeErrorException("Failed to create VkInstance!");
        }
    }

    void Context::CreateDebugMessenger(const RendererCreateInfo* const pRendererCreateInfo)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(Context::CreateDebugMessenger());
        VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo{};
        DebugMessenger::PopulateDebugCreateInfo(&debugMessengerCreateInfo);
        m_DebugMessenger = std::make_unique<DebugMessenger>(m_Instance, &debugMessengerCreateInfo, pRendererCreateInfo->m_pAllocationCallbacks);
    }

    void Context::PopulateAdditionalInstanceInfo(
        VkInstanceCreateInfo *pInstanceCreateInfo)
    {}
} // namespace Savanna::Gfx::Vk
