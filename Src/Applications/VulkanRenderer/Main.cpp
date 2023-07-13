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

#include "SavannaEngine.h"
#include <Utilities/Console.h>

int main(int argc, char** argvs)
{
    try
    {
        using namespace Savanna::Application;
        std::unique_ptr<VulkanApplication> app = std::unique_ptr<VulkanApplication>(new VulkanApplication());

        app->Run();
    }
    catch(const std::exception& e)
    {
        SAVANNA_FATAL_LOG("{}", e.what());
        return 1;
    }

    return 0;
}
