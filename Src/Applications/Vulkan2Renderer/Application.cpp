#include "Application.h"

#include <Gfx/Vk/Public/ISavannaGfxVk2.h>

using namespace Savanna;

Application::Application(const char *rootPath)
    : m_Window(1920, 1080)
{
    SAVANNA_INSERT_SCOPED_PROFILER(Application::Application);
    SavannaInitialize();
    SavannaStart();
    Savanna::IO::VirtualFileSystem::Construct(rootPath);

    m_FailedInitialization = !TryInitGfx();
    if (m_FailedInitialization)
    {
        SAVANNA_LOG("Failed to initialize application.");
        return;
    }

    SAVANNA_LOG("Application created.");
}

Application::~Application()
{
    SAVANNA_INSERT_SCOPED_PROFILER(Application::~Application);
    Savanna::Gfx::Shutdown();
    Savanna::IO::VirtualFileSystem::Destroy();
    SavannaStop();
    SavannaShutdown();
}

void Application::Run()
{
    if (m_FailedInitialization)
        return;

    SAVANNA_LOG("Application running.");
    while (!m_Window.ShouldClose())
    {
        glfwPollEvents();
    }
}

bool Application::TryInitGfx()
{
    SAVANNA_INSERT_SCOPED_PROFILER(Application::TryInitGfx);
    auto defaultAllocatorInterface = Savanna::MemoryManager::GetAllocatorInterfaceForLabel(k_SavannaMemoryLabelGfx);
    se_GfxContextCreateInfo_t gfxContextCreateInfo
    {
        .m_pApplicationName = "SavannaVk2",
        .m_Allocator = defaultAllocatorInterface,
        .m_pUserData = nullptr,
    };

    if (SAVANNA_GFX_FAILURE(Gfx::Initialize(&gfxContextCreateInfo)))
    {
        SAVANNA_LOG("Failed to initialize graphics context.");
        return false;
    }

    const char* defaultLayers[]
    {
        "VK_LAYER_KHRONOS_validation"
    };

    const char* defaultDeviceExtensions[]
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    };

    uint32 instanceExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&instanceExtensionCount);

    se_VkDriverCreateInfo_t vkDriverCreateInfo {};
    vkDriverCreateInfo.m_pNext = nullptr;
    vkDriverCreateInfo.m_pUserData = nullptr;

    // TODO @david.mohrhardt: Setup input arguments for application to enable/disable validation layers.
    vkDriverCreateInfo.m_InstanceCreateArgs.m_ppEnabledLayers = defaultLayers;
    vkDriverCreateInfo.m_InstanceCreateArgs.m_EnabledLayerCount = 1;

    instanceExtensionCount += 1; // For debug utils extension.
    DynamicArray<const char*> instanceExtensions(instanceExtensionCount, defaultAllocatorInterface);
    instanceExtensions.Append(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    instanceExtensions.AppendRange(glfwExtensions, instanceExtensionCount - 1);

    vkDriverCreateInfo.m_InstanceCreateArgs.m_ppEnabledInstanceExtensions = instanceExtensions.Data();
    vkDriverCreateInfo.m_InstanceCreateArgs.m_EnabledInstanceExtensionCount = instanceExtensionCount;

    vkDriverCreateInfo.m_PhysicalDeviceCreateArgs.m_ppEnabledDeviceExtensions = defaultDeviceExtensions;
    vkDriverCreateInfo.m_PhysicalDeviceCreateArgs.m_EnabledDeviceExtensionCount = 1;

    se_GfxDriverCreateInfo_t gfxDriverCreateInfo
    {
        .m_RequestedBackendType = kSavannaGfxApiVulkan,
        .m_Allocator = defaultAllocatorInterface,
        .m_pNext = &vkDriverCreateInfo,
        .m_pUserData = nullptr,
    };

    se_GfxDriverCreateInfoList_t gfxDriverCreateInfoList
    {
        .m_pDriverCreateInfos = &gfxDriverCreateInfo,
        .m_CreateInfoCount = 1,
        .m_pUserData = nullptr,
    };

    if (SAVANNA_GFX_FAILURE(Gfx::CreateDriver(&gfxDriverCreateInfoList)))
    {
        SAVANNA_LOG("Failed to create graphics driver.");
        return false;
    }

    return true;
}
