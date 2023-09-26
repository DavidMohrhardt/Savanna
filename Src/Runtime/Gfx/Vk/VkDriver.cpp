#include "VkDriver.h"

#include "Utilities/VkInfoCreateUtils.h"

namespace Savanna::Gfx::Vk2
{
    se_GfxErrorCode_t VkDriver::Create(const se_GfxDriverCreateInfo_t& createInfo)
    {
        m_AllocatorInterface = createInfo.m_Allocator;

        // Create the instance
        VkApplicationInfo appInfo = Utils::k_SavannaDefaultVulkanAppInfo;
        // VkInstanceCreateInfo instanceCreateInfo = Utils::PopulateInstanceCreateInfo(appInfo);

        return kSavannaGfxErrorCodeSuccess;
    }

    se_GfxErrorCode_t VkDriver::Destroy()
    {
        return kSavannaGfxErrorCodeSuccess;
    }

} // namespace Savanna::Gfx::VK2


