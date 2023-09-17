/**
 * @file GfxContext.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-09-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>

#include "GfxDriver.h"

#include <Types/Singleton/Singleton.h>

namespace Savanna::Gfx
{
    class GfxContext : public Singleton<GfxContext>
    {
    private:
        DECLARE_FRIENDS_FOR_SINGLETON(GfxContext);

        IGfxDriver* m_pDriver;

        GfxContext(IGfxDriver* pDriver)
            : m_pDriver(pDriver)
        {
            m_pDriver->Create();
        }

        ~GfxContext()
        {
            m_pDriver->Destroy();
        }

    public:
        inline IGfxDriver* GetDriver() const { return m_pDriver; }
    };
} // namespace Savanna::Gfx
