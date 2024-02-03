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

namespace savanna::gfx
{
    static consteval size_t GetGfxDriverInterfaceSizeInPtrs()
    {
        return sizeof(seGfxDriverInterface) / sizeof(void*);
    }

    void GfxDriver::SetInterface(seGfxDriverInterface& interface)
    {
        ClearInterface();
        m_pInterface = m_Allocator.New<seGfxDriverInterface>();
        if (m_pInterface != nullptr)
        {
            memcpy(m_pInterface, &interface, sizeof(seGfxDriverInterface));
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

    seGfxErrorCode GfxDriver::Create(const seGfxDriverCreateInfo& createInfo)
    {
        if (m_pInterface == nullptr)
        {
            return kSavannaGfxErrorCodeInvalidDriverInterface;
        }
        return (*m_pInterface).m_pfnInitialize(createInfo);
    }

    seGfxErrorCode GfxDriver::Destroy()
    {
        if (m_pInterface == nullptr)
        {
            return kSavannaGfxErrorCodeInvalidDriverInterface;
        }
        return (*m_pInterface).m_pfnDestroy();
    }

    seGfxDriver GfxDriver::GetDriver()
    {
        if (m_pInterface == nullptr)
        {
            return kSavannaGfxErrorCodeInvalidDriverInterface;
        }
        return (*m_pInterface).m_pfnGetDriver();
    }

    seGfxErrorCode GfxDriver::CreateSwapchain(const seGfxSwapchainCreateInfo& createInfo, seGfxHandle* const pOutSwapchainHandle)
    {
        if (m_pInterface == nullptr)
        {
            return kSavannaGfxErrorCodeInvalidDriverInterface;
        }
        return (*m_pInterface).m_pfnCreateSwapchain(createInfo, pOutSwapchainHandle);
    }

    const seGfxBackend GfxDriver::GetBackendType() const
    {
        if (m_pInterface == nullptr)
        {
            return kSavannaGfxApiNone;
        }
        return (*m_pInterface).m_pfnGetBackend();
    }

    seGfxErrorCode GfxDriver::CreateShaderModule(
        const seGfxShaderCreateInfo& createInfo,
        seGfxShader& outShaderModuleHandle)
    {
        if (m_pInterface == nullptr)
        {
            return kSavannaGfxErrorCodeInvalidDriverInterface;
        }
        return (*m_pInterface).m_pfnCreateShaderModule(createInfo, outShaderModuleHandle);
    }

    seJobHandle GfxDriver::CreateShaderModulesAsync(
        const seGfxShaderCreateInfo* pCreateInfos,
        const size_t createInfoCount,
        seGfxShader** const ppOutShaderModuleHandles)
    {
        if (m_pInterface == nullptr || (*m_pInterface).m_pfnCreateShaderModulesAsync == nullptr)
        {
            return k_InvalidJobHandle;
        }

        return (*m_pInterface).m_pfnCreateShaderModulesAsync(pCreateInfos, createInfoCount, ppOutShaderModuleHandles);
    }
} // namespace savanna::Gfx
