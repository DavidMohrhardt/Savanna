#include "VkShader.h"

#include "VkShaderCreateJob.h"

namespace Savanna::Gfx::Vk
{
    VkShader::VkShader()
        : m_Device()
        , m_ShaderModule(VK_NULL_HANDLE)

    {
        m_ShaderModule = VK_NULL_HANDLE;
    }

    VkShader::VkShader(const VkDevice& device, const char* shaderPath)
        : m_Device(device)
        , m_ShaderModule(VK_NULL_HANDLE)
    {
        IO::FileStream shaderFileStream(shaderPath);
        auto shaderCode = shaderFileStream.ReadFile();

        VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
        shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderModuleCreateInfo.codeSize = shaderCode.size();
        shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

        m_ShaderModule = Create(device, shaderModuleCreateInfo);
    }

    VkShader::VkShader(const VkDevice& device, const byte* shaderCode, const size_t shaderCodeSize)
        : m_Device(device)
        , m_ShaderModule(VK_NULL_HANDLE)
    {
        VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
        shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderModuleCreateInfo.codeSize = shaderCodeSize;
        shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode);

        m_ShaderModule = Create(nullptr, shaderModuleCreateInfo);
    }

    VkShader::VkShader(const VkDevice& device, const VkShaderModuleCreateInfo& shaderModuleCreateInfo)
        : m_Device(device)
        , m_ShaderModule(Create(device, shaderModuleCreateInfo))
    {
    }

    VkShader::VkShader(const VkShader& other) SAVANNA_NO_EXCEPT
    {
        m_Device = other.m_Device;
        m_ShaderModule = other.m_ShaderModule;
    }

    VkShader::VkShader(VkShader&& other) SAVANNA_NO_EXCEPT
    {
        m_Device = other.m_Device;
        m_ShaderModule = other.m_ShaderModule;

        other.m_Device = VK_NULL_HANDLE;
        other.m_ShaderModule = VK_NULL_HANDLE;
    }

    VkShader::~VkShader()
    {
        if (m_Device != VK_NULL_HANDLE && m_ShaderModule != VK_NULL_HANDLE)
            vkDestroyShaderModule(m_Device, m_ShaderModule, nullptr);
    }

    VkShaderModule VkShader::Create(const VkDevice& device, const VkShaderModuleCreateInfo& shaderModuleCreateInfo)
    {
        VkShaderModuleCreationJob shaderCreateJob {
            .m_Device = device,
            .m_ShaderModuleCreateInfo = shaderModuleCreateInfo
        };

        // TODO (David Mohrhardt): When jobs are async use job.schdule() and job.wait() instead of job.execute()
#if SAVANNA_JOB_SYSTEM_ASYNC

        #error "REMOVE ME"
        using namespace Concurrency;
        auto handle = JobSystem::Get().Schedule(shaderCreateJob);
        JobSystem::Get().Wait(handle);

#else // !SAVANNA_JOB_SYSTEM_ASYNC

        shaderCreateJob.Execute();

#endif // SAVANNA_JOB_SYSTEM_ASYNC

        return shaderCreateJob.m_OutShaderModule;
    }
} // namespace Savanna::Gfx::Vulkan
