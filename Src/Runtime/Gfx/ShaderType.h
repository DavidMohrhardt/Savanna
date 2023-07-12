/**
 * @file ShaderKind.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-01-27
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>

#include <Types/Enums/Enumeration.h>

DECLARE_NAMESPACED_ENUMERATION(Savanna::Gfx, seShaderType_t, ShaderType, uint8_t,
    kSavannaGfxInvalidShader = 0,
    kSavannaGfxVertexShader,
    kSavannaGfxFragmentShader,
    kSavannaGfxGeometryShader,
    kSavannaGfxTesselationControlShader,
    kSavannaGfxTesselationEvaluationShader,
    kSavannaGfxComputeShader
);
