#include "VkGpu.h"

namespace Savanna::Gfx::Vk2
{
    void VkGpu::Reset(VkInstance& instance)
    {
        if (m_LogicalDevice != VK_NULL_HANDLE)
        {
            vkDestroyDevice(m_LogicalDevice, nullptr);
            m_LogicalDevice = VK_NULL_HANDLE;
        }
        m_PhysicalDevice = VK_NULL_HANDLE;
        m_PhysicalDeviceProperties = {};
    }
} // namespace Savanna::Gfx::Vk2
