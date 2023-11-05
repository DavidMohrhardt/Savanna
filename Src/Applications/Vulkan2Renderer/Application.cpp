#include "Application.h"

using namespace Savanna;

Application::Application(const char *rootPath)
    : m_Window(1920, 1080)
{
    SAVANNA_INSERT_SCOPED_PROFILER(Application::Application);
    SavannaInitialize();
    SavannaStart();
    Savanna::IO::VirtualFileSystem::Construct(rootPath);

    m_pRenderer = new vk::Renderer();
    if (m_pRenderer->TryInitialize(nullptr, m_Window.GetWindowPtr()))
    {
        SAVANNA_LOG("Renderer initialized.");
    }
    else
    {
        // throw Savanna::RuntimeErrorException("Failed to initialize renderer.");
        SAVANNA_LOG("Failed to initialize renderer.");
    }

    SAVANNA_LOG("Application created.");
}

Application::~Application()
{
    SAVANNA_INSERT_SCOPED_PROFILER(Application::~Application);

    delete m_pRenderer;

    Savanna::Gfx::Shutdown();
    Savanna::IO::VirtualFileSystem::Destroy();

    SavannaStop();
    SavannaShutdown();
}

void Application::Run()
{
    SAVANNA_LOG("Application running.");
    while (!m_Window.ShouldClose())
    {
        glfwPollEvents();
    }
}
