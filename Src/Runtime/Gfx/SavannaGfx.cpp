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
#include "Public/ISavannaGfx.hpp"

namespace Savanna::Gfx
{
    const se_AllocatorInterface_t GetDefaultAllocatorInterface()
    {
        static se_AllocatorInterface_t k_DefaultGraphicsAllocatorInterface = MemoryManager::GetAllocatorInterfaceForLabel(k_SavannaMemoryLabelGfx);
        return k_DefaultGraphicsAllocatorInterface;
    }

    GfxErrorCode Initialize(const se_GfxContextCreateInfo_t *const pCreateInfo)
    {
        auto pContext = GfxContext::Construct(pCreateInfo);
        return pContext != nullptr
            ? kSavannaGfxErrorCodeSuccess
            : kSavannaGfxErrorCodeGfxNotInitialized;
    }

    GfxErrorCode Shutdown()
    {
        if (auto pContext = GfxContext::Get())
        {
            GfxContext::Destroy();
            return kSavannaGfxErrorCodeSuccess;
        }
        return kSavannaGfxErrorCodeGfxNotInitialized;
    }

    GfxErrorCode CreateDriver(const se_GfxDriverCreateInfoList_t* const pCreateInfoList)
    {
        if (auto pContext = GfxContext::Get())
        {
            return pContext->CreateDriver(pCreateInfoList);
        }
        return kSavannaGfxErrorCodeGfxNotInitialized;
    }

    GfxSupportedBackend GetSupportedGfxBackends()
    {
        if (auto pContext = GfxContext::Get())
        {
            return pContext->GetSupportedGfxBackends();
        }
        return kSavannaSupportedGfxApiNone;
    }

    GfxBackend GetActiveGfxBackend()
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
} // namespace Savanna::Gfx

SAVANNA_EXPORT(se_GfxErrorCode_t) SavannaGfxInit(const se_GfxContextCreateInfo_t* const pCreateInfo)
{
    return Savanna::Gfx::Initialize(pCreateInfo);
}

SAVANNA_EXPORT(se_GfxErrorCode_t) SavannaGfxShutdown()
{
    return Savanna::Gfx::Shutdown();
}

SAVANNA_EXPORT(se_GfxErrorCode_t) SavannaGfxContextCreateDriver(
    const se_GfxDriverCreateInfoList_t* const pCreateInfoList)
{
    return Savanna::Gfx::CreateDriver(pCreateInfoList);
}

SAVANNA_EXPORT(se_GfxSupportedBackend_t) SavannaGfxGetSupportedGraphicsBackends()
{
    return Savanna::Gfx::GetSupportedGfxBackends();
}

SAVANNA_EXPORT(se_GfxBackend_t) SavannaGfxGetActiveGraphicsBackend()
{
    return Savanna::Gfx::GetActiveGfxBackend();
}
