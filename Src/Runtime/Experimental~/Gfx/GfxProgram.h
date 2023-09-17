/**
 * @file GfxProgram.h
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

#include "Types/Containers/Arrays/DynamicArray.h"

#include "GfxShader.h"

DECLARE_NAMESPACED_ENUMERATION(Savanna::Gfx, GfxProgramType, GfxProgramType, uint8_t,
    k_SavannaGfxProgramTypeUnknown = 0,

    k_SavannaGfxProgramTypeVertex,
    k_SavannaGfxProgramTypeFragment,
    k_SavannaGfxProgramTypeCompute,
    k_SavannaGfxProgramTypeGeometry,
    k_SavannaGfxProgramTypeTessellationControl,
    k_SavannaGfxProgramTypeTessellationEvaluation

    // k_SavannaGfxProgramTypeRayGeneration,
    // k_SavannaGfxProgramTypeRayIntersection,
    // k_SavannaGfxProgramTypeRayAnyHit,
    // k_SavannaGfxProgramTypeRayClosestHit,
    // k_SavannaGfxProgramTypeRayMiss,
    // k_SavannaGfxProgramTypeRayCallable

    k_SavannaGfxProgramTypeCount
);

typedef struct se_GfxProgramCreateInfo_t
{
    GfxProgramType type;
    const GfxShader* pShader;
} se_GfxProgramCreateInfo_t;

typedef struct se_GfxProgram_t
{
    GfxProgramType type;
    const GfxShader* pShader;
} se_GfxProgram_t;

typedef se_uint64_t se_GfxProgramHandle_t;
