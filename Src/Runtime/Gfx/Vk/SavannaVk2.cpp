#include "SavannaVk2.h"

#include "VkDriver.h"

namespace savanna::gfx::vk
{
    se_VkGraphicsCapabilities_t g_GraphicsCapabilities {};

    void InitializeGraphicsCapabilities(VkInstance instance, VkPhysicalDevice physicalDevice)
    {
        // g_GraphicsCapabilities.m_IsSupported = true;
    }

    se_GfxErrorCode_t GetErrorCode(VkResult result)
    {
        switch (result)
        {
            default:
                return kSavannaGfxErrorCodeUnknownError;
        }
    }
} // namespace savanna::gfx::vk

se_GfxErrorCode_t GetDriverInterfaceVulkan(se_GfxDriverInterface_t& outDriverInterface)
{
    using namespace savanna::gfx::vk;
    VkDriver::PopulateDriverInterface(outDriverInterface);
    return kSavannaGfxErrorCodeSuccess;
}
