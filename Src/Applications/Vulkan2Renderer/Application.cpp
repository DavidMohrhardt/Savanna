#include "Application.h"

#include <Gfx/Vk/Public/ISavannaGfxVk2.h>

using namespace Savanna;

Application::Application(const char *rootPath)
    : m_Window(1920, 1080)
{
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
    Savanna::Gfx::Shutdown();
    Savanna::IO::VirtualFileSystem::Destroy();
    SavannaStop();
    SavannaShutdown();
    SAVANNA_LOG("Application destroyed.");
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
    const char* defaultLayers[]
    {
        "VK_LAYER_KHRONOS_validation"
    };
    const char* defaultInstanceExtensions[]
    {
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
    };
    const char* defaultDeviceExtensions[]
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    };

    auto defaultGfxAllocator = Savanna::Gfx::GetDefaultAllocatorInterface();
    se_GfxContextCreateInfo_t gfxContextCreateInfo
    {
        .m_pApplicationName = "SavannaVk2",
        .m_Allocator = defaultGfxAllocator,
        .m_pUserData = nullptr,
    };

    if (SAVANNA_GFX_FAILURE(Gfx::Initialize(&gfxContextCreateInfo)))
    {
        SAVANNA_LOG("Failed to initialize graphics context.");
        return false;
    }

    se_VkDriverCreateInfo_t vkDriverCreateInfo {};
    vkDriverCreateInfo.m_pNext = nullptr;
    vkDriverCreateInfo.m_pUserData = nullptr;

    vkDriverCreateInfo.m_InstanceCreateArgs.m_ppEnabledLayers = defaultLayers;
    vkDriverCreateInfo.m_InstanceCreateArgs.m_EnabledLayerCount = 1;
    vkDriverCreateInfo.m_InstanceCreateArgs.m_ppEnabledInstanceExtensions = defaultInstanceExtensions;
    vkDriverCreateInfo.m_InstanceCreateArgs.m_EnabledInstanceExtensionCount = 1;

    vkDriverCreateInfo.m_PhysicalDeviceCreateArgs.m_ppEnabledDeviceExtensions = defaultDeviceExtensions;
    vkDriverCreateInfo.m_PhysicalDeviceCreateArgs.m_EnabledDeviceExtensionCount = 1;

    se_GfxDriverCreateInfo_t gfxDriverCreateInfo
    {
        .m_RequestedBackendType = kSavannaGfxApiVulkan,
        .m_Allocator = defaultGfxAllocator,
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
