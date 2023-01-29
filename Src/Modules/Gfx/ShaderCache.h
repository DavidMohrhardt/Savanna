/**
 * @file ShaderCache.h
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

#include <Types/Singleton/Singleton.h>

namespace Savanna::Gfx
{
    class ShaderCache : public Singleton<ShaderCache>
    {
        friend class IShaderCache;

    private:
        std::unordered_map<GfxApi, std::unique_ptr<IShaderCache>> m_ShaderCaches;

    public:
        ShaderCache();
        ~ShaderCache();

    public:
        // Shader GetShader(const char* shaderPath, ShaderType shaderType);
        // Shader GetShader(const byte* shaderCode, const size_t shaderCodeSize, ShaderType shaderType);

    private:
        bool TryRegisterShaderCacheForPlatform(GfxApi platformApi, IShaderCache* shaderCache);
    };
} // namespace Savanna::Gfx
