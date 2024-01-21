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
    inline GfxErrorCode Initialize(const seGfxContextCreateInfo *const pCreateInfo)
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

    inline GfxErrorCode CreateDriver(const seGfxDriverCreateInfoList* const pCreateInfoList)
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

    inline const seGfxHandle GetDriverHandle()
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

    inline GfxErrorCode CreateSwapchain(const seGfxSwapchainCreateInfo* const pCreateInfo, seGfxHandle* const pOutSwapchainHandle)
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

    inline GfxErrorCode CreateShaderModule(const seGfxShaderCreateInfo& createInfo, seGfxShaderHandle& outShaderModuleHandle)
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

    inline seJobHandle CreateShaderModulesAsync(
        const seGfxShaderCreateInfo* pCreateInfos,
        const size_t createInfoCount,
        seGfxShaderHandle** const ppOutShaderModuleHandles)
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
SAVANNA_EXPORT(seGfxErrorCode) SavannaGfxInit(const seGfxContextCreateInfo* const pCreateInfo)
{
    return savanna::gfx::Initialize(pCreateInfo);
}

SAVANNA_EXPORT(seGfxErrorCode) SavannaGfxShutdown()
{
    return savanna::gfx::Shutdown();
}

SAVANNA_EXPORT(seGfxErrorCode) SavannaGfxContextCreateDriver(
    const seGfxDriverCreateInfoList* const pCreateInfoList)
{
    return savanna::gfx::CreateDriver(pCreateInfoList);
}

SAVANNA_EXPORT(seGfxSupportedBackend) SavannaGfxGetSupportedGraphicsBackends()
{
    return savanna::gfx::GetSupportedGfxBackends();
}

SAVANNA_EXPORT(seGfxBackend) SavannaGfxGetActiveGraphicsBackend()
{
    return savanna::gfx::GetActiveGfxBackend();
}

SAVANNA_EXPORT(seGfxHandle) SavannaGfxGetDriverHandle()
{
    return savanna::gfx::GetDriverHandle();
}

SAVANNA_EXPORT(seGfxErrorCode) SavannaGfxCreateSwapchain(
    const seGfxSwapchainCreateInfo* const pCreateInfo,
    seGfxHandle* const pOutSwapchainHandle)
{
    return savanna::gfx::CreateSwapchain(pCreateInfo, pOutSwapchainHandle);
}

SAVANNA_EXPORT(seGfxErrorCode) SavannaGfxCreateShaderModule(
    const seGfxShaderCreateInfo& createInfo,
    seGfxShaderHandle& outShaderModuleHandle)
{
    return savanna::gfx::CreateShaderModule(createInfo, outShaderModuleHandle);
}

SAVANNA_EXPORT(seJobHandle) SavannaGfxCreateShaderModulesAsync(
    const seGfxShaderCreateInfo* pCreateInfos,
    const size_t createInfoCount,
    seGfxShaderHandle** const ppOutShaderModuleHandles)
{
    return savanna::gfx::CreateShaderModulesAsync(pCreateInfos, createInfoCount, ppOutShaderModuleHandles);
}
