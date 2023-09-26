#include "VkDriver.h"

#include "Utilities/VkInfoCreateUtils.h"

namespace Savanna::Gfx::Vk2
{
    void VkDriver::Create(const se_GfxDriverCreateInfo_t& createInfo)
    {
        m_AllocatorInterface = createInfo.m_Allocator;

        // Create the instance
        VkApplicationInfo appInfo = Utils::k_SavannaDefaultVulkanAppInfo;
        // VkInstanceCreateInfo instanceCreateInfo = Utils::PopulateInstanceCreateInfo(appInfo);
    }

    void VkDriver::Destroy() {}

} // namespace Savanna::Gfx::VK2


