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

// TODO @DavidMohrhardt: Move this to public interface definition

#include <SavannaEngine.h>
#include "Public/ISavannaGfx.h"

namespace Savanna::Gfx
{
    class IGfxDriver
    {
    public:
        IGfxDriver() = default;
        virtual ~IGfxDriver() = default;

        virtual se_GfxErrorCode_t Create(const se_GfxDriverCreateInfo_t& createInfo) = 0;
        virtual se_GfxErrorCode_t Destroy() = 0;
        virtual se_GfxBackend_t GetBackendType() const = 0;
    };

    /**
     * @brief Provides a simplified base class for any given GfxDriver.
     *
     * @tparam BACKEND The GfxBackend this driver is implemented against.
     * @note This class is not intended to be used directly, but rather to be inherited from.
     */
    template <se_GfxBackend_t BACKEND>
    class GfxDriverBase : public IGfxDriver
    {
    public:
        GfxDriverBase() = default;
        virtual ~GfxDriverBase() = default;

        se_GfxBackend_t GetBackendType() const final { return BACKEND; }
    };

    class GenericGfxDriver final : public GfxDriverBase<kSavannaGfxExternalBackend>
    {
    private:
        friend class GfxContext;

        static se_GfxErrorCode_t CreateDriver(
            const se_GfxDriverCreateInfo_t& createInfo,
            IGfxDriver** ppDriver,
            void* pUserData);

        se_GfxDriverInterface_t m_Interface;

    public:
        GenericGfxDriver(se_GfxDriverInterface_t interface)
            : m_Interface(interface)
        {}

        ~GenericGfxDriver() = default;

        se_GfxErrorCode_t Create(const se_GfxDriverCreateInfo_t& createInfo) override
        {
            return m_Interface.m_pfnCreate(createInfo);
        }

        se_GfxErrorCode_t Destroy() override
        {
            return m_Interface.m_pfnDestroy();
        }
    };
} // namespace Savanna::Gfx

#endif // !SAVANNA_GFX_DRIVER_H
