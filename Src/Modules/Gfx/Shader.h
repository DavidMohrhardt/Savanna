/**
 * @file Shader.h
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

#include "ShaderType.h"

namespace Savanna::Gfx
{
    class Shader : public NonCopyable
    {
    private:
        uint64_t m_ShaderGUID = 0;
        ShaderType m_ShaderType = kSavannaGfxInvalidShader;

        // Include Shader Descriptor here

    public:
        Shader() = default;
        Shader(Shader&& other);

        Shader(uint64_t shaderId, ShaderType shaderType);
        ~Shader();

    public:
        Shader& operator=(Shader&& other);

    public:
        inline uint64_t GetShaderGUID() const { return m_ShaderGUID; }
        inline ShaderType GetShaderType() const { return m_ShaderType; }

    public:
        void* GetPlatformShader() const;
        bool IsValid() const;
    };
} // namespace Savanna::Gfx
