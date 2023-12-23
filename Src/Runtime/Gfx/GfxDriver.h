/**
 * @file GfxDriver.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-09-15
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef SAVANNA_GFX_DRIVER_H
#define SAVANNA_GFX_DRIVER_H

#include <SavannaEngine.h>

#include "SavannaGfx.h"

#include <Concurrency/SavannaConcurrency.h>

namespace savanna::Gfx
{
    class GfxDriver
    {
    private:
        friend class GfxContext;

        se_GfxDriverInterface_t* m_pInterface = nullptr;
        InterfaceAllocator m_Allocator;

        void SetInterface(se_GfxDriverInterface_t& driverInterface);
        void ClearInterface();

    public:
        GfxDriver(const se_AllocatorInterface_t& allocatorInterface)
            : m_Allocator(allocatorInterface)
        {}

        GfxDriver() = default;
        GfxDriver(const GfxDriver&) = delete;
        GfxDriver(GfxDriver&&) = delete;
        GfxDriver& operator=(const GfxDriver&) = delete;
        GfxDriver& operator=(GfxDriver&&) = delete;

        ~GfxDriver() = default;

        bool IsValid();

        se_GfxErrorCode_t Create(const se_GfxDriverCreateInfo_t& createInfo);

        se_GfxErrorCode_t Destroy();

        se_GfxDriverHandle_t GetDriverHandle();

        se_GfxErrorCode_t CreateSwapchain(const se_GfxSwapchainCreateInfo_t& createInfo, se_GfxHandle_t* const pOutSwapchainHandle);

        const se_GfxBackend_t GetBackendType() const;

        se_GfxErrorCode_t CreateShaderModule(
            const se_GfxShaderCreateInfo_t& createInfo,
            se_GfxShaderHandle_t& outShaderModuleHandle);

        se_JobHandle_t CreateShaderModulesAsync(
            const se_GfxShaderCreateInfo_t* pCreateInfos,
            const size_t createInfoCount,
            se_GfxShaderHandle_t** const ppOutShaderModuleHandles);
    };
} // namespace savanna::Gfx

#endif // !SAVANNA_GFX_DRIVER_H
