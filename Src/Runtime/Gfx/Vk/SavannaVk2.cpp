#include "SavannaVk2.h"

namespace Savanna::Gfx::Vk2
{
    se_GfxErrorCode_t CreateDriver(
        const se_GfxDriverCreateInfo_t *const pCreateInfo,
        IGfxDriver **ppDriver,
        void *pUserData)
    {
        if (!ppDriver)
        {
            return kSavannaGfxErrorCodeInvalidArgument;
        }

        InterfaceAllocator allocator(pCreateInfo->m_Allocator);
        *ppDriver = allocator.New<VkDriver>();
        if (!*ppDriver)
        {
            return kSavannaGfxErrorCodeOutOfMemory;
        }

        (*ppDriver)->Create(*pCreateInfo);
        return kSavannaGfxErrorCodeSuccess;
    }
} // namespace Savanna::Gfx::Vk2
