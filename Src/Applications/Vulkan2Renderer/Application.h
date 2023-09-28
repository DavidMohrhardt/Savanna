#pragma once

#include <SavannaEngine.h>
#include <Gfx/Public/ISavannaGfx.hpp>
#include <IO/VirtualFileSystem.h>

#include "Window/GLFWWindowWrapper.h"

class Application
{
private:
    bool m_FailedInitialization = false;
    GLFWWindowWrapper m_Window;

public:
    Application() = delete;

    Application(const char* rootPath);
    ~Application();

    void Run();

private:
    bool TryInitGfx();
};
