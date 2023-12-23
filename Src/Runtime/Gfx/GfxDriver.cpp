/**
 * @file GfxDriver.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-09-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "GfxDriver.h"

#include "Concurrency/SavannaConcurrency.h"

namespace savanna::Gfx
{
    static consteval size_t GetGfxDriverInterfaceSizeInPtrs()
    {
        return sizeof(se_GfxDriverInterface_t) / sizeof(void*);
    }

    void GfxDriver::SetInterface(se_GfxDriverInterface_t& interface)
    {
        ClearInterface();
        m_pInterface = m_Allocator.New<se_GfxDriverInterface_t>();
        if (m_pInterface != nullptr)
        {
            memcpy(m_pInterface, &interface, sizeof(se_GfxDriverInterface_t));
        }
    }

    void GfxDriver::ClearInterface()
    {
        if (m_pInterface != nullptr)
        {
            m_Allocator.Delete(m_pInterface);
            m_pInterface = nullptr;
        }
    }

    bool GfxDriver::IsValid() {
        constexpr size_t k_InterfaceSizeInPtrs = GetGfxDriverInterfaceSizeInPtrs();
        return m_pInterface != nullptr;
    }

    se_GfxErrorCode_t GfxDriver::Create(const se_GfxDriverCreateInfo_t& createInfo)
    {
        if (m_pInterface == nullptr)
        {
            return kSavannaGfxErrorCodeInvalidDriverInterface;
        }
        return (*m_pInterface).m_pfnInitialize(createInfo);
    }

    se_GfxErrorCode_t GfxDriver::Destroy()
    {
        if (m_pInterface == nullptr)
        {
            return kSavannaGfxErrorCodeInvalidDriverInterface;
        }
        return (*m_pInterface).m_pfnDestroy();
    }

    se_GfxDriverHandle_t GfxDriver::GetDriverHandle()
    {
        if (m_pInterface == nullptr)
        {
            return kSavannaGfxErrorCodeInvalidDriverInterface;
        }
        return (*m_pInterface).m_pfnGetDriverHandle();
    }

    se_GfxErrorCode_t GfxDriver::CreateSwapchain(const se_GfxSwapchainCreateInfo_t& createInfo, se_GfxHandle_t* const pOutSwapchainHandle)
    {
        if (m_pInterface == nullptr)
        {
            return kSavannaGfxErrorCodeInvalidDriverInterface;
        }
        return (*m_pInterface).m_pfnCreateSwapchain(createInfo, pOutSwapchainHandle);
    }

    const se_GfxBackend_t GfxDriver::GetBackendType() const
    {
        if (m_pInterface == nullptr)
        {
            return kSavannaGfxApiNone;
        }
        return (*m_pInterface).m_pfnGetBackend();
    }

    se_GfxErrorCode_t GfxDriver::CreateShaderModule(
        const se_GfxShaderCreateInfo_t& createInfo,
        se_GfxShaderHandle_t& outShaderModuleHandle)
    {
        if (m_pInterface == nullptr)
        {
            return kSavannaGfxErrorCodeInvalidDriverInterface;
        }
        return (*m_pInterface).m_pfnCreateShaderModule(createInfo, outShaderModuleHandle);
    }

    se_JobHandle_t GfxDriver::CreateShaderModulesAsync(
        const se_GfxShaderCreateInfo_t* pCreateInfos,
        const size_t createInfoCount,
        se_GfxShaderHandle_t** const ppOutShaderModuleHandles)
    {
        if (m_pInterface == nullptr || (*m_pInterface).m_pfnCreateShaderModulesAsync == nullptr)
        {
            return k_InvalidJobHandle;
        }

        return (*m_pInterface).m_pfnCreateShaderModulesAsync(pCreateInfos, createInfoCount, ppOutShaderModuleHandles);
    }
} // namespace savanna::Gfx
