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

    uint32 instanceExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&instanceExtensionCount);

    se_VkDriverCreateInfo_t vkDriverCreateInfo {};
    vkDriverCreateInfo.m_pNext = nullptr;
    vkDriverCreateInfo.m_pUserData = nullptr;
    vkDriverCreateInfo.m_EnableValidationLayers = true;
    vkDriverCreateInfo.m_EnableSurfaceExtension = true;

    // TODO @david.mohrhardt: Setup input arguments for application to enable/disable validation layers.
    vkDriverCreateInfo.m_InstanceCreateArgs.m_ppEnabledInstanceExtensions = glfwExtensions;
    vkDriverCreateInfo.m_InstanceCreateArgs.m_EnabledInstanceExtensionCount = instanceExtensionCount;

    vkDriverCreateInfo.m_PhysicalDeviceCreateArgs.m_PreferredGraphicsDeviceIndex = -1;

    vkDriverCreateInfo.m_LogicalDeviceCreateArgs.m_ppEnabledDeviceExtensions = nullptr;
    vkDriverCreateInfo.m_LogicalDeviceCreateArgs.m_EnabledDeviceExtensionCount = 0;

    se_GfxDriverCreateInfo_t gfxDriverCreateInfo
    {
        .m_RequestedBackendType = kSavannaGfxApiVulkan,
        .m_AllocatorInterface = defaultAllocatorInterface,
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
