/**
 * @file GfxContext.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
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

namespace savanna::gfx
{
    class GfxContext : public Singleton<GfxContext>
    {
    private:
        DECLARE_FRIENDS_FOR_SINGLETON(GfxContext);

        InterfaceAllocator m_Allocator;
        GfxDriver m_Driver;

        GfxContext() = delete;

        GfxContext(const seGfxContextCreateInfo* const pCreateInfo);
        ~GfxContext();

    public:
        seGfxErrorCode CreateDriver(const seGfxDriverCreateInfoList* const pCreateInfoList);
        GfxDriver* GetDriver() { return &m_Driver; }

        seGfxSupportedBackend GetSupportedGfxBackends() const;
    };
} // namespace savanna::Gfx

#endif // !SAVANNA_GFX_CONTEXT_H
