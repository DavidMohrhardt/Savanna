/**
 * @file GfxCapabilities.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief Defines the common capabilities of the current graphics API.
 * @version 0.1
 * @date 2023-09-17
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>

#include "GfxApi.h"

typedef struct GfxCapabilities
{
    se_GfxSupportedBackend_t m_SupportedGfxApi;
    se_GfxBackend_t m_ActiveGfxApi;

    void* pPlatformSpecificCapabilities;
} GfxCapabilities;

SAVANNA_EXPORT(const GfxCapabilities&) Savanna_Gfx_GetCapabilities();
