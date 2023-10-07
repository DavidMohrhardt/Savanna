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

        InterfaceAllocator m_Allocator;
        GfxDriver m_Driver;

        GfxContext() = delete;

        GfxContext(const se_GfxContextCreateInfo_t* const pCreateInfo);
        ~GfxContext();

    public:
        se_GfxErrorCode_t CreateDriver(const se_GfxDriverCreateInfoList_t* const pCreateInfoList);
        GfxDriver* GetDriver() { return &m_Driver; }

        se_GfxSupportedBackend_t GetSupportedGfxBackends() const;
    };
} // namespace Savanna::Gfx

#endif // !SAVANNA_GFX_CONTEXT_H
