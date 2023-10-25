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

namespace Savanna::Gfx
{
    const se_AllocatorInterface_t GetDefaultAllocatorInterface()
    {
        static se_AllocatorInterface_t k_DefaultGraphicsAllocatorInterface = MemoryManager::GetAllocatorInterfaceForLabel(k_SavannaMemoryLabelGfx);
        return k_DefaultGraphicsAllocatorInterface;
    }

    GfxErrorCode Initialize(const se_GfxContextCreateInfo_t *const pCreateInfo)
    {
        return GfxContext::Construct(pCreateInfo) != nullptr
            ? kSavannaGfxErrorCodeSuccess
            : kSavannaGfxErrorCodeNotInitialized;
    }

    GfxErrorCode Shutdown()
    {
        if (GfxContext* pContext = GfxContext::Get()) SAVANNA_BRANCH_LIKELY
        {
            GfxContext::Destroy();
            return kSavannaGfxErrorCodeSuccess;
        }
        return kSavannaGfxErrorCodeNotInitialized;
    }

    GfxErrorCode CreateDriver(const se_GfxDriverCreateInfoList_t* const pCreateInfoList)
    {
        if (GfxContext* pContext = GfxContext::Get()) SAVANNA_BRANCH_LIKELY
        {
            return pContext->CreateDriver(pCreateInfoList);
        }
        return kSavannaGfxErrorCodeNotInitialized;
    }

    GfxSupportedBackend GetSupportedGfxBackends()
    {
        if (GfxContext* pContext = GfxContext::Get()) SAVANNA_BRANCH_LIKELY
        {
            return pContext->GetSupportedGfxBackends();
        }
        return kSavannaSupportedGfxApiNone;
    }

    const GfxBackend GetActiveGfxBackend()
    {
        if (GfxContext* pContext = GfxContext::Get()) SAVANNA_BRANCH_LIKELY
        {
            if (auto pDriver = pContext->GetDriver()) SAVANNA_BRANCH_LIKELY
            {
                return pDriver->GetBackendType();
            }
        }
        return kSavannaGfxApiNone;
    }

    const se_GfxHandle_t GetDriverHandle()
    {
        if (GfxContext* pContext = GfxContext::Get()) SAVANNA_BRANCH_LIKELY
        {
            if (auto pDriver = pContext->GetDriver()) SAVANNA_BRANCH_LIKELY
            {
                return pDriver->GetDriverHandle();
            }
        }
        return 0x0;
    }

    GfxErrorCode CreateSwapchain(const se_GfxSwapchainCreateInfo_t* const pCreateInfo, se_GfxHandle_t* const pOutSwapchainHandle)
    {
        if (GfxContext* pContext = GfxContext::Get()) SAVANNA_BRANCH_LIKELY
        {
            if (auto pDriver = pContext->GetDriver()) SAVANNA_BRANCH_LIKELY
            {
                if (pCreateInfo == nullptr)
                {
                    return kSavannaGfxErrorCodeInvalidArgument;
                }
                return pDriver->CreateSwapchain(*pCreateInfo, pOutSwapchainHandle);
            }
        }
        return kSavannaGfxErrorCodeNotInitialized;
    }
} // namespace Savanna::Gfx

// Public API
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

SAVANNA_EXPORT(se_GfxHandle_t) SavannaGfxGetDriverHandle()
{
    return Savanna::Gfx::GetDriverHandle();
}

SAVANNA_EXPORT(se_GfxErrorCode_t) SavannaGfxCreateSwapchain(
    const se_GfxSwapchainCreateInfo_t* const pCreateInfo,
    se_GfxHandle_t* const pOutSwapchainHandle)
{
    return Savanna::Gfx::CreateSwapchain(pCreateInfo, pOutSwapchainHandle);
}
