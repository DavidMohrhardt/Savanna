#include "Application.h"

using namespace Savanna;

Application::Application(const char *rootPath)
{
    SavannaInitialize();
    SavannaStart();
    Savanna::IO::VirtualFileSystem::Construct(rootPath);

    if (!TryInitGfx())
    {
        m_FailedInitialization = true;
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

    se_GfxDriverCreateInfo_t gfxDriverCreateInfo
    {
        .m_RequestedBackendType = kSavannaGfxApiVulkan,
        .m_Allocator = defaultGfxAllocator,
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
