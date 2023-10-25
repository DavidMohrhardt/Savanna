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
    auto pDefaultAllocInterface = Savanna::MemoryManager::GetAllocatorInterfaceForLabelPtr(k_SavannaMemoryLabelGfx);
    se_GfxContextCreateInfo_t gfxContextCreateInfo
    {
        .m_pApplicationName = "SavannaVk2",
        .m_pAllocatorInterface = pDefaultAllocInterface,
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
    vkDriverCreateInfo.m_RequestSurface = true;
    vkDriverCreateInfo.m_pWindowHandle =
#if SAVANNA_WINDOWS
        glfwGetWin32Window(m_Window.GetWindowPtr());
#else
        nullptr;
#endif


    vkDriverCreateInfo.m_InstanceCreateArgs.m_ppEnabledInstanceExtensions = glfwExtensions;
    vkDriverCreateInfo.m_InstanceCreateArgs.m_EnabledInstanceExtensionCount = instanceExtensionCount;

    vkDriverCreateInfo.m_PhysicalDeviceCreateArgs.m_PreferredGraphicsDeviceIndex = -1;

    dynamic_array<const char*> enabledDeviceExtensions { 1, k_SavannaMemoryArenaIdGfx };
    if (vkDriverCreateInfo.m_RequestSurface)
    {
        enabledDeviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    }

    vkDriverCreateInfo.m_LogicalDeviceCreateArgs.m_ppEnabledDeviceExtensions = enabledDeviceExtensions.data();
    vkDriverCreateInfo.m_LogicalDeviceCreateArgs.m_EnabledDeviceExtensionCount = enabledDeviceExtensions.size();

    se_GfxDriverCreateInfo_t gfxDriverCreateInfo
    {
        .m_RequestedBackendType = kSavannaGfxApiVulkan,
        .m_pAllocationInterface = pDefaultAllocInterface,
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

    se_GfxSwapchainCreateInfo_t gfxSwapchainCreateInfo {};
    gfxSwapchainCreateInfo.m_ImageCount = 3;
    gfxSwapchainCreateInfo.m_Format = k_SavannaGfxTextureFormat_B8G8R8A8_UNORM;
    glfwGetWindowSize(m_Window.GetWindowPtr(), (int*)&gfxSwapchainCreateInfo.m_Width, (int*)&gfxSwapchainCreateInfo.m_Height);

    se_GfxHandle_t swapchainHandle;
    if (SAVANNA_GFX_FAILURE(Gfx::CreateSwapchain(&gfxSwapchainCreateInfo, &swapchainHandle)))
    {
        SAVANNA_LOG("Failed to request swapchain.");
        return false;
    }

    return true;
}
