#include "SavannaVk2.h"

namespace Savanna::Gfx::Vk2
{
    se_GfxErrorCode_t AcquireDriver(
        const se_GfxDriverCreateInfo_t *const pCreateInfo,
        IGfxDriver **ppDriver,
        void *pUserData,
        const se_AllocatorInterface_t allocatorInterface)
    {
        if (!ppDriver)
        {
            return kSavannaGfxErrorCodeInvalidArgument;
        }

        // Use the given allocator interface to create the driver as it's
        // owned by the caller.
        InterfaceAllocator allocator(allocatorInterface);
        *ppDriver = allocator.New<VkDriver>();
        if (!*ppDriver)
        {
            return kSavannaGfxErrorCodeOutOfMemory;
        }

        return (*ppDriver)->Create(*pCreateInfo);
    }
} // namespace Savanna::Gfx::Vk2
