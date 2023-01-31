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

#include <FileStream.h>

#include <vulkan/vulkan.h>

namespace Savanna::Gfx::Vk
{
    class VkShader
    {
    private:
        VkShaderModule m_ShaderModule;
        VkDevice m_Device;

    public:
        VkShader() SAVANNA_NO_EXCEPT;

        VkShader(const VkDevice& device, const char* shaderPath);
        VkShader(const VkDevice& device, const byte* shaderCode, const size_t shaderCodeSize);
        VkShader(const VkDevice& device, const VkShaderModuleCreateInfo& shaderModuleCreateInfo);

        VkShader(const VkShader& other) SAVANNA_NO_EXCEPT;
        VkShader(VkShader&& other) SAVANNA_NO_EXCEPT;

        ~VkShader();

    public:
        VkShaderModule GetModule() const { return m_ShaderModule; }

    private:
        static VkShaderModule Create(const VkDevice& device, const VkShaderModuleCreateInfo& shaderModuleCreateInfo);
    };
} // namespace Savanna::Gfx::Vulkan
