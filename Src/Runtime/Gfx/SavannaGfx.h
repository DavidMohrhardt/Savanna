#ifndef SAVANNA_GFX_H
#define SAVANNA_GFX_H

#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>

#include <Public/ISavannaGfx.h>

namespace Savanna::Gfx
{
    using GfxErrorCode = Enumeration<se_GfxErrorCode_t, se_uint32>;
    using GfxBackend = Enumeration<se_GfxBackend_t, se_uint32>;
    using GfxSupportedBackend = FlagEnumeration<se_GfxSupportedBackend_t, se_uint8>;

    const se_AllocatorInterface_t GetDefaultAllocatorInterface();

    GfxErrorCode Initialize(const se_GfxContextCreateInfo_t* const pCreateInfo);
    GfxErrorCode Shutdown();

    GfxErrorCode CreateDriver(const se_GfxDriverCreateInfoList_t* const pCreateInfoList);
}

#endif // !SAVANNA_GFX_H
