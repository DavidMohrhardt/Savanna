/**
 * @file Main.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-09-15
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <SavannaEngine.h>

#include <IO/VirtualFileSystem.h>

#include "Application.h"

#include <memory>

int main(int argc, char** argv)
{
    {
        std::unique_ptr<Application> pApp = std::make_unique<Application>(argv[0]);
        try
        {
            pApp->Run();
        }
        catch (const std::exception& e)
        {
            SAVANNA_FATAL_LOG("{}", e.what());
            return 1;
        }
    }

    return 0;
}
