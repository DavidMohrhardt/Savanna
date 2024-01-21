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

namespace savanna::gfx
{
    class GfxDriver
    {
    private:
        friend class GfxContext;

        seGfxDriverInterface* m_pInterface = nullptr;
        InterfaceAllocator m_Allocator;

        void SetInterface(seGfxDriverInterface& driverInterface);
        void ClearInterface();

    public:
        GfxDriver(const seAllocatorInterface& allocatorInterface)
            : m_Allocator(allocatorInterface)
        {}

        GfxDriver() = default;
        GfxDriver(const GfxDriver&) = delete;
        GfxDriver(GfxDriver&&) = delete;
        GfxDriver& operator=(const GfxDriver&) = delete;
        GfxDriver& operator=(GfxDriver&&) = delete;

        ~GfxDriver() = default;

        bool IsValid();

        seGfxErrorCode Create(const seGfxDriverCreateInfo& createInfo);

        seGfxErrorCode Destroy();

        seGfxDriverHandle GetDriverHandle();

        seGfxErrorCode CreateSwapchain(const seGfxSwapchainCreateInfo& createInfo, seGfxHandle* const pOutSwapchainHandle);

        const seGfxBackend GetBackendType() const;

        seGfxErrorCode CreateShaderModule(
            const seGfxShaderCreateInfo& createInfo,
            seGfxShaderHandle& outShaderModuleHandle);

        seJobHandle CreateShaderModulesAsync(
            const seGfxShaderCreateInfo* pCreateInfos,
            const size_t createInfoCount,
            seGfxShaderHandle** const ppOutShaderModuleHandles);
    };
} // namespace savanna::Gfx

#endif // !SAVANNA_GFX_DRIVER_H
