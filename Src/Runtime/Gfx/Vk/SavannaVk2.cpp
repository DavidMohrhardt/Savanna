#include "SavannaVk2.h"

#include "VkDriver.h"

namespace savanna::gfx::vk
{
    seVkGraphicsCapabilities g_GraphicsCapabilities {};

    void InitializeGraphicsCapabilities(VkInstance instance, VkPhysicalDevice physicalDevice)
    {
        // g_GraphicsCapabilities.m_IsSupported = true;
    }

    seGfxErrorCode GetErrorCode(VkResult result)
    {
        switch (result)
        {
            default:
                return kSavannaGfxErrorCodeUnknownError;
        }
    }
} // namespace savanna::gfx::vk

seGfxErrorCode GetDriverInterfaceVulkan(seGfxDriverInterface& outDriverInterface)
{
    using namespace savanna::gfx::vk;
    VkDriver::PopulateDriverInterface(outDriverInterface);
    return kSavannaGfxErrorCodeSuccess;
}
