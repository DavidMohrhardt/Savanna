/**
 * @file VkShaderCache.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-05-18
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "VkShaderCache.h"

#include <JobSystem/JobManager.h>

using namespace Savanna::Concurrency;

namespace Savanna::Gfx::Vk
{
    struct ShaderModuleCreationJob : public IJob
    {
        friend class ShaderProgram;

    public:
        ShaderCache* m_pShaderCache;
        const FixedString64 m_ShaderName;
        VkDevice m_Device;
        std::vector<const byte> m_ShaderBinary;
        // VkShaderModuleCreateInfo m_ShaderModuleCreateInfo;
        VkShaderModule m_OutShaderModule;

    public:
        ShaderModuleCreationJob(
            ShaderCache* pShaderCache,
            const FixedString64 &shaderName,
            const VkDevice& device,
            // const VkShaderModuleCreateInfo& shaderModuleCreateInfo) SAVANNA_NOEXCEPT
            const std::vector<const byte>&& shaderBinary) SAVANNA_NOEXCEPT
            : m_pShaderCache(pShaderCache)
            , m_ShaderName(shaderName)
            , m_Device(device)
            // , m_ShaderModuleCreateInfo(shaderModuleCreateInfo)
            , m_ShaderBinary(std::move(shaderBinary))
            , m_OutShaderModule()
        {}

    public:
        virtual JobResult Execute() SAVANNA_OVERRIDE
        {
            SAVANNA_ASSERT(m_pShaderCache != nullptr, "Shader cache is null!");
            SAVANNA_ASSERT(m_Device != VK_NULL_HANDLE, "Device is null!");

            VkShaderModuleCreateInfo shaderModuleCreateInfo{};
            shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
            shaderModuleCreateInfo.codeSize = m_ShaderBinary.size();
            shaderModuleCreateInfo.pCode = static_cast<const uint32_t*>(m_ShaderBinary.data());
            shaderModuleCreateInfo.flags = 0;

            VkShaderModule shaderModule{};
            VkResult result = vkCreateShaderModule(m_Device, &shaderModuleCreateInfo, nullptr, &shaderModule);
            if (result != VK_SUCCESS)
            {
                SAVANNA_WARNING_LOG("Failed to create shader module! Error code: {}!", ResultToString(result));
                return k_SavannaJobResultError;
            }

            m_pShaderCache->RegisterShaderModule(m_ShaderName, shaderModule);
            m_OutShaderModule = shaderModule;

            return k_SavannaJobResultSuccess;
        }
    };

    bool ShaderCache::TryCreateShader(
        const FixedString64 &shaderName,
        const VkDevice &device,
        std::vector<const byte>&& shaderBinary)
    {
        VkShaderModule shaderModule;
        VkResult result = vkCreateShaderModule(device, &shaderModuleCreateInfo, nullptr, &shaderModule);
        if (result != VK_SUCCESS)
        {
            SAVANNA_WARNING_LOG("Failed to create shader module! Error code: {}!", ResultToString(result));
            return false;
        }

        RegisterShaderModule(shaderName, shaderModule);
        return true;
    }

    JobHandle ShaderCache::TryCreateShaderAsync(
        const FixedString64& shaderName,
        const VkDevice& device,
        std::vector<const byte>&& shaderBinary)
        // const VkShaderModuleCreateInfo& shaderModuleCreateInfo)
    {
        // m_ShaderModuleMap[shaderName] = VK_NULL_HANDLE;
        return JobManager::Get()->ScheduleJob(
            new AutomaticJob<ShaderModuleCreationJob>(this, shaderName, device, std::move(shaderBinary)),
            JobPriority::k_SavannaJobPriorityHigh);
    }
} // namespace Savanna::Gfx::Vk
