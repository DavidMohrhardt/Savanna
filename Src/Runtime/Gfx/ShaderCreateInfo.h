#pragma once

#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>

#include "ShaderType.h"

typedef struct se_ShaderDescriptor_t
{
    se_uint64_t m_ShaderGUID = 0;
    se_ShaderType_t m_ShaderType = kSavannaGfxInvalidShader;
    se_ShaderLanguage_t m_ShaderLanguage = kSavannaGfxInvalidShaderLanguage;
    se_SupportedGfxApi_t m_SupportedGfxApi = kSavannaGfxInvalidApi;
} se_ShaderDescriptor_t;
DECLARE_SAVANNA_EXTENDED_NAMESPACED_CPP_TYPE_DEF(Gfx, se_ShaderDescriptor_t, ShaderDescriptor);

typedef struct se_ShaderCreateInfo_t
{
    se_ShaderDescriptor_t m_ShaderDescriptor;
    void* m_pShaderData = nullptr;
    // Include Shader Descriptor here
} se_ShaderCreateInfo_t;
DECLARE_SAVANNA_EXTENDED_NAMESPACED_CPP_TYPE_DEF(Gfx, se_ShaderCreateInfo_t, ShaderCreateInfo);
