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
#ifndef SAVANNA_GFX_CONTEXT_H
#define SAVANNA_GFX_CONTEXT_H

#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>

#include "SavannaGfx.h"

#include "GfxDriver.h"

#include <Types/Singleton/Singleton.h>

namespace Savanna::Gfx
{
    class GfxContext : public Singleton<GfxContext>
    {
    private:
        DECLARE_FRIENDS_FOR_SINGLETON(GfxContext);

        se_AllocatorInterface_t m_AllocatorInterface;
        IGfxDriver* m_pDriver;

        GfxContext() = default;
        GfxContext(const se_GfxContextCreateInfo_t* const pCreateInfo);
        ~GfxContext();

    public:
        se_GfxErrorCode_t CreateDriver(const se_GfxDriverCreateInfoList_t* const pCreateInfoList);
        inline IGfxDriver* GetDriver() const { return m_pDriver; }

        se_GfxSupportedBackend_t GetSupportedGfxBackends() const;
    };
} // namespace Savanna::Gfx

#endif // !SAVANNA_GFX_CONTEXT_H
