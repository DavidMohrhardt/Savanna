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

int main(int argc, char** argvs)
{
    int result = 0;

    Savanna::InitializeManagers();
    Savanna::StartManagers();

    VulkanApplication* app = SAVANNA_NEW(VulkanApplication, argc, argvs);
    try
    {
        app->Run();
    }
    catch(const std::exception& e)
    {
        SAVANNA_FATAL_LOG("{}", e.what());
        result = -1;
    }

    SAVANNA_DELETE(app);

    Savanna::StopManagers();
    Savanna::ShutdownManagers();

    return result;
}
