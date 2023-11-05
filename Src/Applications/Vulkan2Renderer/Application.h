#pragma once

#include <SavannaEngine.h>

#include <Gfx/SavannaGfx.h>
#include <IO/VirtualFileSystem.h>

#include "Renderer.h"
#include "Window/GLFWWindowWrapper.h"

class Application
{
private:
    vk::Renderer* m_pRenderer;
    GLFWWindowWrapper m_Window;

public:
    Application() = delete;

    Application(const char* rootPath);
    ~Application();

    void Run();

private:
    bool TryInitGfx();
};
