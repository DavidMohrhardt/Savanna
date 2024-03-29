#include "SavannaVk2.h"

#include "VkDriver.h"

namespace Savanna::Gfx::Vk2
{
    se_VkGraphicsCapabilities_t g_GraphicsCapabilities {};

    se_GfxErrorCode_t GetDriverInterface(se_GfxDriverInterface_t& outDriverInterface)
        // const se_GfxDriverCreateInfo_t *const pCreateInfo,
        // se_GfxDriverInterface_t& outDriverInterface,
        // void *pUserData,
        // const se_AllocatorInterface_t allocatorInterface)
    {
        VkDriver::PopulateDriverInterface(outDriverInterface);
        return kSavannaGfxErrorCodeSuccess;
    }

    void InitializeGraphicsCapabilities(VkInstance instance, VkPhysicalDevice physicalDevice)
    {
        // g_GraphicsCapabilities.m_IsSupported = true;
    }
} // namespace Savanna::Gfx::Vk2
