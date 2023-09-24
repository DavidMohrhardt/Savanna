/**
 * @file SavannaGfx.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-09-19
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "SavannaGfx.h"
#include "GfxContext.h"

using namespace Savanna::Gfx;

SAVANNA_EXPORT(void) se_GfxInit()
{
    GfxContext::Construct();
}

SAVANNA_EXPORT(void) se_GfxShutdown()
{
    GfxContext::Destroy();
}

SAVANNA_EXPORT(se_GfxErrorCode_t) se_GfxContextCreateDriver(
    const se_GfxDriverCreateInfo_t* const pCreateInfo)
{
    if (auto pContext = GfxContext::Get())
    {
        return pContext->CreateDriver(pCreateInfo);
    }
    return kSavannaGfxErrorCodeGfxNotInitialized;
}

SAVANNA_EXPORT(se_GfxSupportedGfxBackend_t) se_GfxGetSupportedGraphicsBackends()
{
    if (auto pContext = GfxContext::Get())
    {
        return pContext->GetSupportedGfxBackends();
    }
    return kSavannaSupportedGfxApiNone;
}

SAVANNA_EXPORT(se_GfxBackend_t) se_GfxGetActiveGraphicsApi()
{
    if (auto pContext = GfxContext::Get())
    {
        if (auto pDriver = pContext->GetDriver())
        {
            return pDriver->GetBackendType();
        }
    }
    return kSavannaGfxApiNone;
}
