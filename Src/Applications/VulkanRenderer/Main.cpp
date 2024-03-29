/**
 * @file Main.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief Implements the "Main" function for the Application.
 * @version 0.1
 * @date 2022-08-02
 *
 */

#include <memory>

#include "VulkanApplication.h"

#include <SavannaEngine.h>

#include <Utilities/Console.h>
#include <VirtualFileSystem.h>

int main(int argc, char** argvs)
{
    int result = 0;

    using namespace Savanna;

    SavannaInitialize();
    SavannaStart();

    // Initialize File System
    IO::VirtualFileSystem::Construct(argvs[0]);

    VulkanApplication* app = SAVANNA_NEW(k_SavannaMemoryLabelHeap, VulkanApplication, argc, argvs);
    try
    {
        app->Run();
    }
    catch(const std::exception& e)
    {
        SAVANNA_FATAL_LOG("{}", e.what());
        result = -1;
    }

    SAVANNA_DELETE(k_SavannaMemoryLabelHeap, app);

    IO::VirtualFileSystem::Destroy();

    SavannaStop();
    SavannaShutdown();

    return result;
}
