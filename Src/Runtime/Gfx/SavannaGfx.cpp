/**
 * @file SavannaGfx.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-09-19
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "SavannaGfx.h"
#include "GfxContext.h"

#include "Concurrency/JobSystem.h"

namespace savanna::gfx
{
    inline GfxErrorCode Initialize(const se_GfxContextCreateInfo_t *const pCreateInfo)
    {
        return GfxContext::Construct(pCreateInfo) != nullptr
            ? kSavannaGfxErrorCodeSuccess
            : kSavannaGfxErrorCodeNotInitialized;
    }

    inline GfxErrorCode Shutdown()
    {
        if (GfxContext* pContext = GfxContext::Get()) SAVANNA_BRANCH_LIKELY
        {
            GfxContext::Destroy();
            return kSavannaGfxErrorCodeSuccess;
        }
        return kSavannaGfxErrorCodeNotInitialized;
    }

    inline GfxErrorCode CreateDriver(const se_GfxDriverCreateInfoList_t* const pCreateInfoList)
    {
        if (GfxContext* pContext = GfxContext::Get()) SAVANNA_BRANCH_LIKELY
        {
            return pContext->CreateDriver(pCreateInfoList);
        }
        return kSavannaGfxErrorCodeNotInitialized;
    }

    inline GfxSupportedBackend GetSupportedGfxBackends()
    {
        if (GfxContext* pContext = GfxContext::Get()) SAVANNA_BRANCH_LIKELY
        {
            return pContext->GetSupportedGfxBackends();
        }
        return kSavannaSupportedGfxApiNone;
    }

    inline const GfxBackend GetActiveGfxBackend()
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

    inline const se_GfxHandle_t GetDriverHandle()
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

    inline GfxErrorCode CreateSwapchain(const se_GfxSwapchainCreateInfo_t* const pCreateInfo, se_GfxHandle_t* const pOutSwapchainHandle)
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

    inline GfxErrorCode CreateShaderModule(const se_GfxShaderCreateInfo_t& createInfo, se_GfxShaderHandle_t& outShaderModuleHandle)
    {
        if (GfxContext* pContext = GfxContext::Get()) SAVANNA_BRANCH_LIKELY
        {
            if (auto pDriver = pContext->GetDriver()) SAVANNA_BRANCH_LIKELY
            {
                return pDriver->CreateShaderModule(createInfo, outShaderModuleHandle);
            }
        }
        return kSavannaGfxErrorCodeNotInitialized;
    }

    inline se_JobHandle_t CreateShaderModulesAsync(
        const se_GfxShaderCreateInfo_t* pCreateInfos,
        const size_t createInfoCount,
        se_GfxShaderHandle_t** const ppOutShaderModuleHandles)
    {
        if (GfxContext* pContext = GfxContext::Get()) SAVANNA_BRANCH_LIKELY
        {
            if (auto pDriver = pContext->GetDriver()) SAVANNA_BRANCH_LIKELY
            {
                return pDriver->CreateShaderModulesAsync(pCreateInfos, createInfoCount, ppOutShaderModuleHandles);
            }
        }
        return kSavannaGfxErrorCodeNotInitialized;
    }
} // namespace savanna::Gfx

// Public API
SAVANNA_EXPORT(se_GfxErrorCode_t) SavannaGfxInit(const se_GfxContextCreateInfo_t* const pCreateInfo)
{
    return savanna::gfx::Initialize(pCreateInfo);
}

SAVANNA_EXPORT(se_GfxErrorCode_t) SavannaGfxShutdown()
{
    return savanna::gfx::Shutdown();
}

SAVANNA_EXPORT(se_GfxErrorCode_t) SavannaGfxContextCreateDriver(
    const se_GfxDriverCreateInfoList_t* const pCreateInfoList)
{
    return savanna::gfx::CreateDriver(pCreateInfoList);
}

SAVANNA_EXPORT(se_GfxSupportedBackend_t) SavannaGfxGetSupportedGraphicsBackends()
{
    return savanna::gfx::GetSupportedGfxBackends();
}

SAVANNA_EXPORT(se_GfxBackend_t) SavannaGfxGetActiveGraphicsBackend()
{
    return savanna::gfx::GetActiveGfxBackend();
}

SAVANNA_EXPORT(se_GfxHandle_t) SavannaGfxGetDriverHandle()
{
    return savanna::gfx::GetDriverHandle();
}

SAVANNA_EXPORT(se_GfxErrorCode_t) SavannaGfxCreateSwapchain(
    const se_GfxSwapchainCreateInfo_t* const pCreateInfo,
    se_GfxHandle_t* const pOutSwapchainHandle)
{
    return savanna::gfx::CreateSwapchain(pCreateInfo, pOutSwapchainHandle);
}

SAVANNA_EXPORT(se_GfxErrorCode_t) SavannaGfxCreateShaderModule(
    const se_GfxShaderCreateInfo_t& createInfo,
    se_GfxShaderHandle_t& outShaderModuleHandle)
{
    return savanna::gfx::CreateShaderModule(createInfo, outShaderModuleHandle);
}

SAVANNA_EXPORT(se_JobHandle_t) SavannaGfxCreateShaderModulesAsync(
    const se_GfxShaderCreateInfo_t* pCreateInfos,
    const size_t createInfoCount,
    se_GfxShaderHandle_t** const ppOutShaderModuleHandles)
{
    return savanna::gfx::CreateShaderModulesAsync(pCreateInfos, createInfoCount, ppOutShaderModuleHandles);
}
