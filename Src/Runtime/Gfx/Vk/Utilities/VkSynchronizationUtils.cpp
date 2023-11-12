#include "VkSynchronizationUtils.h"



namespace Savanna::Gfx::Vk2::Utils
{
    const char* k_SynchronizationExtensions[]
    {
#if VK_KHR_synchronization2
        VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME,
        VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME,
#endif
    };

    void VkSynchronizationInfo::Initialize(
        const VkGpu &gpu, const char **availableDeviceExt)
    {

    }
} // namespace Savanna::Gfx::Vk2::Utils
