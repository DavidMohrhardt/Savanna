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
    static se_GfxErrorCode_t CreateSwapchain(const se_GfxSwapchainCreateInfo_t& createInfo, se_GfxHandle_t* const pOutSwapchainHandle)

// TODO @DavidMohrhardt: Move this to public interface definition

#include <SavannaEngine.h>
#include "Public/ISavannaGfx.h"

namespace Savanna::Gfx
{
    class GfxDriver
    {
    private:
        friend class GfxContext;

        se_GfxDriverInterface_t m_Interface {};
        void SetInterface(se_GfxDriverInterface_t& interface) { m_Interface = interface; }
        void ClearInterface() { m_Interface = {}; }

    public:
        GfxDriver(se_GfxDriverInterface_t interface)
            : m_Interface(interface)
        {}

        GfxDriver() = default;
        GfxDriver(const GfxDriver&) = delete;
        GfxDriver(GfxDriver&&) = delete;
        GfxDriver& operator=(const GfxDriver&) = delete;
        GfxDriver& operator=(GfxDriver&&) = delete;

        ~GfxDriver() = default;

        bool IsValid();

        se_GfxErrorCode_t Create(const se_GfxDriverCreateInfo_t& createInfo)
        {
            return m_Interface.m_pfnInitialize(createInfo);
        }

        se_GfxErrorCode_t Destroy()
        {
            return m_Interface.m_pfnDestroy();
        }

        se_GfxDriverHandle_t GetDriverHandle()
        {
            return m_Interface.m_pfnGetDriverHandle();
        }

        se_GfxErrorCode_t CreateSwapchain(const se_GfxSwapchainCreateInfo_t& createInfo, se_GfxHandle_t* const pOutSwapchainHandle)
        {
            return m_Interface.m_pfnCreateSwapchain(createInfo, pOutSwapchainHandle);
        }

        const se_GfxBackend_t GetBackendType() const
        {
            return m_Interface.m_pfnGetBackend();
        }
    };
} // namespace Savanna::Gfx

#endif // !SAVANNA_GFX_DRIVER_H
