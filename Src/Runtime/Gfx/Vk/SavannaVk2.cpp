#include "SavannaVk2.h"

#include "VkDriver.h"

namespace savanna::Gfx::Vk2
{
    se_VkGraphicsCapabilities_t g_GraphicsCapabilities {};

    void InitializeGraphicsCapabilities(VkInstance instance, VkPhysicalDevice physicalDevice)
    {
        // g_GraphicsCapabilities.m_IsSupported = true;
    }
} // namespace savanna::Gfx::Vk2

se_GfxErrorCode_t GetDriverInterfaceVulkan(se_GfxDriverInterface_t& outDriverInterface)
{
    using namespace savanna::Gfx::Vk2;
    VkDriver::PopulateDriverInterface(outDriverInterface);
    return kSavannaGfxErrorCodeSuccess;
}
