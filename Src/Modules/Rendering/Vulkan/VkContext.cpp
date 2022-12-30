#include "VkContext.h"


#include "VkDebugMessenger.h"
#include "VkRendererCreateInfo.h"

#include "Utilities/VkRendererInitializationCache.h"

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

    Context::Context(const RendererCreateInfo* const pCreateInfo)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(Context::Context(const RendererCreateInfo *const pCreateInfo));
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = *pCreateInfo->m_ApplicationName;
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = *pCreateInfo->m_EngineName;
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo instanceCreateInfo{};
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pApplicationInfo = &appInfo;
        instanceCreateInfo.enabledExtensionCount = pCreateInfo->m_InstanceExtensionsCount;
        instanceCreateInfo.ppEnabledExtensionNames = pCreateInfo->m_InstanceExtensions;

        CreateVkInstance(&instanceCreateInfo);
        CreateDebugMessenger(pCreateInfo);
    }

    Context::~Context()
    {
        if (m_Instance != VK_NULL_HANDLE)
            vkDestroyInstance(m_Instance, nullptr);

        m_DebugMessenger = nullptr;
    }

    Context& Context::operator=(Context &&other)
    {
        m_Instance = other.m_Instance;
        other.m_Instance = nullptr;

        m_DebugMessenger = std::move(other.m_DebugMessenger);

        return *this;
    }

    void Context::CreateVkInstance(VkInstanceCreateInfo *pInstanceCreateInfo)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(Context::CreateVkInstance(VkInstanceCreateInfo *pInstanceCreateInfo));

        PopulateAdditionalInstanceInfo(pInstanceCreateInfo);

        if (vkCreateInstance(pInstanceCreateInfo, nullptr, &m_Instance) != VK_SUCCESS)
        {
            throw Savanna::RuntimeErrorException("Failed to create VkInstance!");
        }

        Utils::RendererInitializationCache::Get()->SetVkInstance(m_Instance);
    }

    void Context::CreateDebugMessenger(const RendererCreateInfo* const pRendererCreateInfo)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(Context::CreateDebugMessenger());
        VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo{};
        DebugMessenger::PopulateDebugCreateInfo(&debugMessengerCreateInfo);
        m_DebugMessenger = std::make_unique<DebugMessenger>(m_Instance, &debugMessengerCreateInfo, pRendererCreateInfo->m_pAllocationCallbacks);
    }

    inline void Context::PopulateAdditionalInstanceInfo(VkInstanceCreateInfo *pInstanceCreateInfo)
    {
        using namespace Utils;

        SAVANNA_INSERT_SCOPED_PROFILER(Context::PopulateAdditionalInstanceInfo(VkInstanceCreateInfo *pInstanceCreateInfo, void *userData));

        auto pInitializationMetadata = RendererInitializationCache::Get();
        pInstanceCreateInfo->enabledExtensionCount = static_cast<uint32_t>(pInitializationMetadata->GetEnabledInstanceExtensionCount());
        pInstanceCreateInfo->ppEnabledExtensionNames = pInitializationMetadata->GetEnabledInstanceExtensionNames();
        pInstanceCreateInfo->enabledLayerCount = static_cast<uint32_t>(pInitializationMetadata->GetEnabledInstanceLayerCount());
        pInstanceCreateInfo->ppEnabledLayerNames = pInitializationMetadata->GetEnabledInstanceLayerNames();
    }
} // namespace Savanna::Gfx::Vk
