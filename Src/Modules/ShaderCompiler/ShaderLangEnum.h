/**
 * @file ShaderLangEnum.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief Defines an enum for shader languages.
 * @version 0.1
 * @date 2023-01-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>
#include <Types/Enums/Enumeration.h>

typedef enum se_ShaderLanguageId
{
    kShaderLanaguageIdUnknown,
    kShaderLanaguageIdSPIRV,
    kShaderLanaguageIdHLSL,
    kShaderLanaguageIdGLSL,
    kShaderLanaguageIdCG,
} se_ShaderLanguageId;
DECLARE_NAMESPACED_ENUMERATION(Savanna::Toolchain, se_ShaderLanguageId, ShaderLanguageId, se_uint32);
