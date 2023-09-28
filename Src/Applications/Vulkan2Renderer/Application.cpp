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
    auto defaultGfxAllocator = Savanna::Gfx::GetDefaultAllocatorInterface();
    se_GfxContextCreateInfo_t gfxContextCreateInfo
    {
        .m_Allocator = defaultGfxAllocator,
        .m_pUserData = nullptr,
    };

    if (SAVANNA_GFX_FAILURE(Gfx::Initialize(&gfxContextCreateInfo)))
    {
        SAVANNA_LOG("Failed to initialize graphics context.");
        return false;
    }

    {
        se_VkDriverCreateInfo_t vkDriverCreateInfo
        {
            nullptr,
            0,
            nullptr,
            0,
            nullptr,
            0,
            nullptr,
            0,
        };

        DynamicArray<const char*> enabledFeatures(
        {
            VK_KHR_SURFACE_EXTENSION_NAME,
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
            "VK_LAYER_KHRONOS_validation",
        }, defaultGfxAllocator);

        vkDriverCreateInfo.m_ppEnabledInstanceExtensions = enabledFeatures.Data();
        vkDriverCreateInfo.m_EnabledInstanceExtensionCount = 1;

        vkDriverCreateInfo.m_ppEnabledDeviceExtensions = &enabledFeatures.Data()[1];
        vkDriverCreateInfo.m_EnabledDeviceExtensionCount = 1;

        vkDriverCreateInfo.m_ppEnabledLayers = &enabledFeatures.Data()[2];
        vkDriverCreateInfo.m_EnabledLayerCount = 1;

        se_GfxDriverCreateInfo_t gfxDriverCreateInfo
        {
            .m_RequestedBackendType = kSavannaGfxApiVulkan,
            .m_Allocator = defaultGfxAllocator,
            .m_pRealDriverCreateInfo = &vkDriverCreateInfo,
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
    }

    return true;
}
