/**
 * @file Main.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief Implements the "Main" function for the Application.
 * @version 0.1
 * @date 2022-08-02
 *
 */

#include "VulkanApplication.h"

#include "SavannaEngine.h"

int main(int argc, char** argvs)
{
    try
    {
        SavannaVulkan::VulkanApplication app = SavannaVulkan::VulkanApplication();
        app.Run();
    }
    catch(const std::exception& e)
    {
        SAVANNA_FATAL_LOG("%s", e.what());
        return 1;
    }

    return 0;
}
