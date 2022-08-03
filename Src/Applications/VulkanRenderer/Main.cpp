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