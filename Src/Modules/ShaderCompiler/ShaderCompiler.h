/**
 * @file ShaderCompiler.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief Provides a class that can compile shaders into the proper format for consumption by a renderer.
 * @version 0.1
 * @date 2023-01-15
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>

#include <fstream>
#include <filesystem>
#include <memory>
#include <unordered_map>
#include <vector>

#include "IPlatformShaderCompiler.h"

namespace Savanna::Toolchain
{
    class ShaderCompiler
    {
    private:
        std::vector<std::shared_ptr<IPlatformShaderCompiler>> m_spPlatformShaderCompilers;

    public:
        ShaderCompiler() = default;
        ~ShaderCompiler() = default;

        Promise<std::pair<HashString, void*>> ScheduleShaderCompilation(const std::filesystem::path& shader_path, const HashString& shaderId);
    private:
        std::pair<HashString, void*> CompileShaderImpl(const std::filesystem::path& shader_path, const HashString& shaderId, const ShaderLangSpecifier shaderLangSpecifier);
    };
} // namespace Savanna::Toolchain
