#pragma once

#include <SavannaEngine.h>
#include <Gfx/Public/ISavannaGfx.hpp>
#include <IO/VirtualFileSystem.h>

class Application
{
public:
    Application() = delete;

    Application(const char* rootPath);
    ~Application();

    void Run();

private:
    bool TryInitGfx();

    bool m_FailedInitialization = false;
};
