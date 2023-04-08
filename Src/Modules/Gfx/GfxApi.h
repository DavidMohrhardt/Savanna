/**
 * @file GfxApiEnumeration.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief Provides enumeration values indicating supported/active graphics APIs
 * @version 0.1
 * @date 2022-08-02
 *
 */

#pragma once

#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>

DEFINE_SAVANNA_ENUM(Savanna::Gfx, se_GfxApi_t, GfxApi, se_uint32,
    kSavannaGfxApiNone = 0,
    kSavannaGfxApiVulkan,
    kSavannaGfxApiDirectX12,
    kSavannaGfxApiDirectX11,
    kSavannaGfxApiOpenGL,
    kSavannaGfxApiOpenGLES,
    kSavannaGfxApiMetal,
);

DEFINE_SAVANNA_FLAG_ENUMERATION(Savanna::Gfx, se_GfxSupportedGfxApi_t, SupportedGraphicsApi, se_uint32,
    kSavannaSupportedGfxApiNone = 0,
    kSavannaSupportedGfxApiAll = ~0,

    kSavannaSupportedGfxApiVulkan = 1 << kSavannaGfxApiVulkan,
    kSavannaSupportedGfxApiDirectX12 = 1 << kSavannaGfxApiDirectX12,
    kSavannaSupportedGfxApiDirectX11 = 1 << kSavannaGfxApiDirectX11,
    kSavannaSupportedGfxApiOpenGL = 1 << kSavannaGfxApiOpenGL,
    kSavannaSupportedGfxApiOpenGLES = 1 << kSavannaGfxApiOpenGLES,
    kSavannaSupportedGfxApiMetal = 1 << kSavannaGfxApiMetal,
);
