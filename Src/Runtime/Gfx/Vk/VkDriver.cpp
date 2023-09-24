#include "VkDriver.h"

#include <Types/Memory/InterfaceAllocator.h>

namespace Savanna::Gfx::Vk2
{
    se_GfxErrorCode_t CreateDriver(
        const se_GfxDriverCreateInfo_t& createInfo,
        IGfxDriver ** ppDriver,
        void * pUserData)
    {
        if (!ppDriver)
        {
            return kSavannaGfxErrorCodeInvalidArgument;
        }

        InterfaceAllocator allocator(createInfo.m_Allocator);
        *ppDriver = allocator.New<VkDriver>();
        if (!*ppDriver)
        {
            return kSavannaGfxErrorCodeOutOfMemory;
        }

        (*ppDriver)->Create(createInfo);
        return kSavannaGfxErrorCodeSuccess;
    }

    void VkDriver::Create(const se_GfxDriverCreateInfo_t& createInfo)
    {

    }

    void VkDriver::Destroy() {}

} // namespace Savanna::Gfx::VK2


