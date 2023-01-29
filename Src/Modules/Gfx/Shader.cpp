/**
 * @file Shader.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-01-27
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "Shader.h"

namespace Savanna::Gfx
{
    Shader::Shader(uint64_t shaderId, ShaderType shaderType)
        : m_ShaderGUID(shaderId)
        , m_ShaderType(shaderType)
    {
    }

    Shader::Shader(Shader &&other)
        : m_ShaderGUID(other.m_ShaderGUID)
        , m_ShaderType(other.m_ShaderType)
    {
        other.m_ShaderGUID = 0;
        other.m_ShaderType = kSavannaGfxInvalidShader;
    }

    Shader::~Shader()
    {
        if (IsValid())
        {
            // ShaderCache::Get()->ReleaseShader(m_ShaderGUID);
        }
    }

    // Operator overloads
    Shader &Shader::operator=(Shader &&other)
    {
        m_ShaderGUID = other.m_ShaderGUID;
        m_ShaderType = other.m_ShaderType;

        other.m_ShaderGUID = 0;
        other.m_ShaderType = kSavannaGfxInvalidShader;

        return *this;
    }

    void *Shader::GetPlatformShader() const
    {
        return nullptr;
        // return ShaderCache::Get()->GetRealShader(m_ShaderGUID);
    }

    bool Shader::IsValid() const
    {
        // return m_ShaderGUID != 0 && m_ShaderType != kSavannaGfxInvalidShader && ShaderCache::Get()->IsShaderValid(m_ShaderGUID);
        return false;
    }
} // namespace Savanna::Gfx
