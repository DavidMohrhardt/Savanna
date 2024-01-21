#ifndef SAVANNA_GFX_H
#define SAVANNA_GFX_H

#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>

#include <Public/ISavannaGfx.hpp>

#define SAVANNA_GFX_RETURN_ERROR_CODE(__Operation) \
    {                                              \
        const seGfxErrorCode __result = __Operation; \
        if (SAVANNA_GFX_FAILURE(__result))         \
        {                                          \
            return __result;                       \
        }                                          \
    }

namespace savanna::gfx
{
    using GfxErrorCode = Enumeration<seGfxErrorCode, se_uint32>;
    using GfxBackend = Enumeration<seGfxBackend, se_uint32>;
    using GfxSupportedBackend = FlagEnumeration<seGfxSupportedBackend, se_uint8>;

    const seAllocatorInterface GetDefaultAllocatorInterface();

    GfxErrorCode Initialize(const seGfxContextCreateInfo* const pCreateInfo);
    GfxErrorCode Shutdown();

    GfxErrorCode CreateDriver(const seGfxDriverCreateInfoList* const pCreateInfoList);
    GfxSupportedBackend GetSupportedGfxBackends();
    const GfxBackend GetActiveGfxBackend();

    GfxErrorCode CreateSwapchain(const seGfxSwapchainCreateInfo* const pCreateInfo, seGfxHandle* const pOutSwapchainHandle);
}

#endif // !SAVANNA_GFX_H
