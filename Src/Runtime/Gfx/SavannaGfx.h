#ifndef SAVANNA_GFX_H
#define SAVANNA_GFX_H

#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>

#include <Public/ISavannaGfx.hpp>

#define SAVANNA_GFX_RETURN_ERROR_CODE(__Operation) \
    {                                              \
        const se_GfxErrorCode_t __result = __Operation; \
        if (SAVANNA_GFX_FAILURE(__result))         \
        {                                          \
            return __result;                       \
        }                                          \
    }

namespace savanna::Gfx
{
    using GfxErrorCode = Enumeration<se_GfxErrorCode_t, se_uint32>;
    using GfxBackend = Enumeration<se_GfxBackend_t, se_uint32>;
    using GfxSupportedBackend = FlagEnumeration<se_GfxSupportedBackend_t, se_uint8>;

    const se_AllocatorInterface_t GetDefaultAllocatorInterface();

    GfxErrorCode Initialize(const se_GfxContextCreateInfo_t* const pCreateInfo);
    GfxErrorCode Shutdown();

    GfxErrorCode CreateDriver(const se_GfxDriverCreateInfoList_t* const pCreateInfoList);
    GfxSupportedBackend GetSupportedGfxBackends();
    const GfxBackend GetActiveGfxBackend();

    GfxErrorCode CreateSwapchain(const se_GfxSwapchainCreateInfo_t* const pCreateInfo, se_GfxHandle_t* const pOutSwapchainHandle);
}

#endif // !SAVANNA_GFX_H
