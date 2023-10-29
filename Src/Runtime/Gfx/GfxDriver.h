/**
 * @file GfxDriver.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-09-15
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef SAVANNA_GFX_DRIVER_H
#define SAVANNA_GFX_DRIVER_H

/**
 * @brief A helper macro for defining the driver interface functions.
 * ensures that each interface function is declared with the correct signature.
 */
#define DECL_GFX_DRIVER_INTERFACE_FUNCS() \
    static se_GfxErrorCode_t Initialize(const se_GfxDriverCreateInfo_t& createInfo); \
    static se_GfxErrorCode_t Destroy(); \
    static se_GfxDriverHandle_t GetDriverHandle(); \
    static se_GfxErrorCode_t CreateSwapchain(const se_GfxSwapchainCreateInfo_t& createInfo, se_GfxHandle_t* const pOutSwapchainHandle); \
    static se_GfxErrorCode_t CreateShaderModule( \
        const se_GfxShaderModuleCreateInfo_t& createInfo, \
        se_GfxShaderModuleHandle_t& outShaderModuleHandle); \
    static se_JobHandle_t CreateShaderModulesAsync( \
        const se_GfxShaderModuleCreateInfo_t* pCreateInfos, \
        const size_t createInfoCount, \
        se_GfxShaderModuleHandle_t** const ppOutShaderModuleHandles)


// TODO @DavidMohrhardt: Move this to public interface definition

#include <SavannaEngine.h>

#include "Public/ISavannaGfx.h"

#include <Concurrency/SavannaConcurrency.h>

namespace Savanna::Gfx
{
    class GfxDriver
    {
    private:
        friend class GfxContext;

        se_GfxDriverInterface_t* m_pInterface = nullptr;
        InterfaceAllocator m_Allocator;

        void SetInterface(se_GfxDriverInterface_t& interface);
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
            const se_GfxShaderModuleCreateInfo_t& createInfo,
            se_GfxShaderModuleHandle_t& outShaderModuleHandle);

        se_JobHandle_t CreateShaderModulesAsync(
            const se_GfxShaderModuleCreateInfo_t* pCreateInfos,
            const size_t createInfoCount,
            se_GfxShaderModuleHandle_t** const ppOutShaderModuleHandles);
    };
} // namespace Savanna::Gfx

#endif // !SAVANNA_GFX_DRIVER_H
