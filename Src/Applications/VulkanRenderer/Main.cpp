/**
 * @file Main.cpp
 * @author David Mohrhardt (david.mohrhardt@gmail.com)
 * @brief Implements the "Main" function for the Application.
 * @version 0.1
 * @date 2022-08-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "VulkanApplication.h"

#include "SavannaEngine.h"
#include "Memory/CacheLine.h"


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