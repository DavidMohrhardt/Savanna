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

int main(int argc, char** argvs)
{
    try
    {
        using namespace SavannaVulkan;
        std::unique_ptr<VulkanApplication> application
            = std::unique_ptr<VulkanApplication>(new VulkanApplication());

        application->Run();
    }
    catch(const std::exception& e)
    {
        SAVANNA_FATAL_LOG("%s", e.what());
        return 1;
    }

    return 0;
}
