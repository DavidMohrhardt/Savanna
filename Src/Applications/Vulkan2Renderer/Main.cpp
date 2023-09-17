/**
 * @file Main.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-09-15
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <SavannaEngine.h>

#include <IO/VirtualFileSystem.h>

#include "Application.h"

void ConstructSavanna(const char* rootPath)
{
    Savanna::InitializeManagers();
    Savanna::StartManagers();
    Savanna::IO::VirtualFileSystem::Construct(rootPath);
}

void TeardownSavanna()
{
    Savanna::IO::VirtualFileSystem::Destroy();
    Savanna::StopManagers();
    Savanna::ShutdownManagers();
}

int main(int argc, char** argv)
{
    ConstructSavanna(argv[0]);

    Application app {};
    try
    {
        app.Run();
    }
    catch (const std::exception& e)
    {
        SAVANNA_FATAL_LOG("{}", e.what());
    }

    TeardownSavanna();
    return 0;
}
