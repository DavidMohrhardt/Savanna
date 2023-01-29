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

DEFINE_SAVANNA_ENUM(Savanna::Gfx, seGfxApi_t, GfxApi, uint32_t,
    kSavannaGfxApiNone = 0,
    kSavannaGfxApiVulkan,
    kSavannaGfxApiDirectX12,
    kSavannaGfxApiDirectX11,
    kSavannaGfxApiOpenGL,
    kSavannaGfxApiOpenGLES,
    kSavannaGfxApiMetal,
    kSavannaGfxApiWebGPU,
);

DEFINE_SAVANNA_FLAG_ENUMERATION(Savanna::Gfx, seGfxSupportedGfxApi_t, SupportedGraphicsApi, uint32_t,
    kSavannaSupportedGfxApiNone = 0,
    kSavannaSupportedGfxApiAll = ~0,

    kSavannaSupportedGfxApiVulkan = 1 << kSavannaGfxApiVulkan,
    kSavannaSupportedGfxApiDirectX12 = 1 << kSavannaGfxApiDirectX12,
    kSavannaSupportedGfxApiDirectX11 = 1 << kSavannaGfxApiDirectX11,
    kSavannaSupportedGfxApiOpenGL = 1 << kSavannaGfxApiOpenGL,
    kSavannaSupportedGfxApiOpenGLES = 1 << kSavannaGfxApiOpenGLES,
    kSavannaSupportedGfxApiMetal = 1 << kSavannaGfxApiMetal,
    kSavannaSupportedGfxApiWebGPU = 1 << kSavannaGfxApiWebGPU,
);
