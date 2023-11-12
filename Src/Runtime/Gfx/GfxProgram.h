/**
 * @file GfxProgram.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-09-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <SavannaEngine.h>

#include "Types/Containers/Arrays/dynamic_array.h"

// #include "GfxShader.h"

struct se_GfxShader_t;

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
    const se_GfxShader_t* pShader;
} se_GfxProgramCreateInfo_t;

typedef struct se_GfxProgram_t
{
    GfxProgramType type;
    const se_GfxShader_t* pShader;
} se_GfxProgram_t;

typedef se_uint64_t se_GfxProgramHandle_t;

#if defined(__cplusplus)

namespace Savanna::Gfx
{
    class GpuProgram
    {
    private:
        friend class IGfxDriver;

        GfxProgramType m_Type;
        const GfxShader* m_pShader;

    public:
        GpuProgram(const GfxProgramType& type, const GfxShader* pShader);
        ~GpuProgram() = default;

        GfxProgramType GetType() const { return m_Type; }
        const GfxShader* GetShader() const { return m_pShader; }
    };
} // namespace Savanna::Gfx


#endif // defined(__cplusplus)
