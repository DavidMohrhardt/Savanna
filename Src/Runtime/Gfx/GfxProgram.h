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

struct seGfxShader;

DECLARE_NAMESPACED_ENUMERATION(savanna::gfx, GfxProgramType, GfxProgramType, uint8_t,
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

typedef struct seGfxProgramCreateInfo
{
    GfxProgramType type;
    const seGfxShader* pShader;
} seGfxProgramCreateInfo;

typedef struct seGfxProgram
{
    GfxProgramType type;
    const seGfxShader* pShader;
} seGfxProgram;

typedef seuint64 seGfxProgramHandle;

#if defined(__cplusplus)

namespace savanna::gfx
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
} // namespace savanna::Gfx


#endif // defined(__cplusplus)
