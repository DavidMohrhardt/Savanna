#pragma once

#include <SavannaEngine.h>

class Application
{
public:
    Application()
    {
        SAVANNA_LOG("Application created.");
    }

    ~Application()
    {
        SAVANNA_LOG("Application destroyed.");
    }

    void Run()
    {
        SAVANNA_LOG("Application running.");
    }
};
