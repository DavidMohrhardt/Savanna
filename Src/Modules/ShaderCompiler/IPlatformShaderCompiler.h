/**
 * @file IShaderCompiler.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief Defines the interface for compiling shaders during runtime from source code.
 * @version 0.1
 * @date 2023-01-15
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>
#include <Types/Strings/HashString.h>

#include <vector>
#include <unordered_map>
#include <filesystem>
#include <fstream>

namespace Savanna::Editor::Toolchain
{
    // class IShaderCompiler
    // {
    // public:
    //     IShaderCompiler() = default;
    //     virtual ~IShaderCompiler() = default;

    //     bool TryCompileShader(const std::filesystem::path& shader_path, const std::vector<HashString>& shader_stage_names);

    // protected:
    //     virtual void CompileShader(const std::filesystem::path& shader_path, const std::vector<HashString>& shader_stage_names) = 0;
    // };
} // namespace Savanna::Editor::Gfx::Compiler
