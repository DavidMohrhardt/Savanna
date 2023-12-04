#include "VkSynchronizationUtils.h"

#include <string>

namespace Savanna::Gfx::Vk2::Utils
{
    VkSynchronizationCapabilities VkSynchronizationInfo::s_Capabilities {};

    const char* k_SynchronizationExtensions[]
    {
#if VK_KHR_synchronization2
        VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME,
        VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME,
#endif
    };

    void VkSynchronizationInfo::Initialize(const VkGpu &gpu, const char** availableDeviceExt, const size_t &extCount)
    {
        s_Capabilities = {};
        for (int i = 0; i < extCount; ++i)
        {
            if (std::strcmp(availableDeviceExt[i], VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME))
            {
                s_Capabilities.m_Synchronization2Supported = true;
            }
            else if (std::strcmp(availableDeviceExt[i], VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME))
            {
                s_Capabilities.m_TimelineSemaphoresSupported = true;
            }
        }
    }
} // namespace Savanna::Gfx::Vk2::Utils
