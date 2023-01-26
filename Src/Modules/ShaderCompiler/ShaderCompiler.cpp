/**
 * @file ShaderCompiler.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief Implements the ShaderCompiler class.
 * @version 0.1
 * @date 2023-01-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "ShaderCompiler.h"

#include <Types/Exceptions/StdExceptionDefinitions.h>

namespace Savanna::Toolchain
{
    Promise<std::pair<HashString, void*>> ShaderCompiler::ScheduleShaderCompilation(const std::filesystem::path& shaderPath, const HashString& shaderId)
    {
        throw NotImplementedException();
        // General Algorithm: Load shader source code from file, once loaded, compile it for each platform, once compiled, return the compiled shader data.
    }
} // namespace Savanna::Toolchain
