#include "VkShader.h"

#include "VkShaderCreateJob.h"

namespace Savanna::Gfx::Vk
{
    JobHandle ScheduleShaderCreateJob(const VkShaderModuleCreateInfo &shaderModuleCreateInfo)
    {
        VkShaderModuleCreationJob shaderModuleCreationJob = {};
        shaderModuleCreationJob.m_ShaderModuleCreateInfo = shaderModuleCreateInfo;
        return shaderModuleCreationJob.Schedule();
    }

    VkShader::VkShader(const byte* shaderCode, const size_t shaderCodeSize)
    {
        VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
        shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderModuleCreateInfo.codeSize = shaderCodeSize;
        shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t *>(shaderCode);

        VkShaderModuleCreationJob shaderModuleCreationJob = {};
        shaderModuleCreationJob.m_ShaderModuleCreateInfo = shaderModuleCreateInfo;

        JobHandle createShaderJob = ScheduleShaderCreateJob(shaderModuleCreateInfo);
        createShaderJob->Wait();

        m_ShaderModule = shaderModuleCreationJob.m_OutShaderModule;
    }

    VkShader::VkShader(const char *shaderPath)
    {
        std::vector<char> shaderCode;
        {
            IO::FileStream shaderStream(shaderPath);
            shaderStream.Read(shaderCode);
        }

        VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
        shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderModuleCreateInfo.codeSize = shaderCode.size();
        shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t *>(shaderCode.data());

        JobHandle createShaderJob = ScheduleShaderCreateJob(shaderModuleCreateInfo);
        createShaderJob->Wait();

        m_ShaderModule = shaderModuleCreationJob.m_OutShaderModule;
    }

    VkShader::VkShader(const VkShaderModuleCreateInfo &shaderModuleCreateInfo)
    {
        JobHandle createShaderJob = ScheduleShaderCreateJob(shaderModuleCreateInfo);
        createShaderJob->Wait();
        m_ShaderModule = shaderModuleCreationJob.m_OutShaderModule;
    }
} // namespace Savanna::Gfx::Vulkan
