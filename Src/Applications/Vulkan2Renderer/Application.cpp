#include "Application.h"

using namespace Savanna;

Application::Application(const char *rootPath)
{
    SavannaInitialize();
    SavannaStart();
    Savanna::IO::VirtualFileSystem::Construct(rootPath);

    se_GfxContextCreateInfo_t gfxContextCreateInfo
    {
        .m_Allocator = Savanna::Gfx::GetDefaultAllocatorInterface(),
        .m_pUserData = nullptr,
    };

    Savanna::Gfx::Initialize(&gfxContextCreateInfo);

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

void Application::Run() { SAVANNA_LOG("Application running."); }
