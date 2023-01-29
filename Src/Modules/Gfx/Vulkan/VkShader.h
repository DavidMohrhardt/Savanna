/**
 * @file VkShader.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-01-26
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>

#include <Public/ISavannaJobs.h>

#include <vulkan/vulkan.h>

namespace Savanna::Gfx::Vk
{
    class VkShader
    {
    private:
        VkShaderModule m_ShaderModule;

    public:
        VkShader(const byte* shaderCode, const size_t shaderCodeSize);
        VkShader(const char* shaderPath);
        VkShader(const VkShaderModuleCreateInfo& shaderModuleCreateInfo);
        VkShader(const VkShader& other) SAVANNA_NO_EXCEPT;
        VkShader(VkShader&& other) SAVANNA_NO_EXCEPT;
        VkShader& operator=(const VkShader& other) = delete;
        ~VkShader();

    public:
        VkShaderModule GetModule() const { return m_ShaderModule; }
    };
} // namespace Savanna::Gfx::Vulkan
