/**
 * @file VkShaderCache.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-01-30
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
    using namespace Savanna::Concurrency;

    class VkShaderCache
    {
    private:
        using namespace Savanna::Concurrency;

        struct ShaderModuleCreationJob : public IJob
        {
            friend class ShaderCache;

        public:
            const ShaderCache* m_pShaderCache;
            VkDevice m_Device;
            VkShaderModuleCreateInfo m_ShaderModuleCreateInfo;
            VkShaderModule* m_pOutShaderModule;

        public:
            VkShaderModuleCreationJob(const VkDevice& device, const VkShaderModuleCreateInfo& shaderModuleCreateInfo, VkShaderModule* pOutShaderModule) SAVANNA_NOEXCEPT
                : m_Device(device)
                , m_ShaderModuleCreateInfo(shaderModuleCreateInfo)
                , m_pOutShaderModule(pOutShaderModule)
            {
            }

        public:
            virtual bool Execute() SAVANNA_OVERRIDE
            {
                SAVANNA_ASSERT(m_pShaderCache != nullptr, "Shader cache is null!");
                SAVANNA_ASSERT(m_Device != VK_NULL_HANDLE, "Device is null!");
                SAVANNA_ASSERT(m_pOutShaderModule != nullptr, "Output shader module is null!");

                VkShaderModule shaderModule;
                VkResult result = vkCreateShaderModule(m_Device, &m_ShaderModuleCreateInfo, nullptr, &shaderModule);
                if (result != VK_SUCCESS)
                {
                    SAVANNA_LOG_ERROR("Failed to create shader module! Error code: {}", result);
                    return false;
                }

                *m_pOutShaderModule = shaderModule;

                return true;
            }
        };

    private:
        VkDevice m_Device;
        std::unordered_map<const int32, VkShaderModule> m_IdToShaderModuleMap;
        std::unordered_map<JobHandle, ShaderModuleCreationJob> m_ActiveJobs;

    public:
        VkShaderCache(const VkDevice& device) SAVANNA_NOEXCEPT;
        ~VkShaderCache();

    public:
        JobHandle CreateShaderModuleAsync(const IHashString* pShaderName, const VkShaderModuleCreateInfo& shaderModuleCreateInfo);
    };

    VkShaderCache::VkShaderCache(const VkDevice& device) SAVANNA_NOEXCEPT
        : m_Device(device)
        , m_IdToShaderModuleMap()
    {
    }

    VkShaderCache::~VkShaderCache()
    {
        auto lambdaJobs = std::vector<LambdaJob>(m_ActiveJobs.size());
        int32 count = 0;
        for (const auto& [jobHandle, shaderModuleCreationJob] : m_ActiveJobs)
        {
            auto lambdaJob = LambdaJob([jobHandle, shaderModuleCreationJob]()
            {
                JobSystem::WaitForJob(jobHandle);
                VkShaderModule module = *shaderModuleCreationJob.m_pOutShaderModule;

                vkDestroyShaderModule(shaderModuleCreationJob.m_Device, *shaderModuleCreationJob.m_pOutShaderModule, nullptr);
                delete shaderModuleCreationJob.m_pOutShaderModule;
            });

            lambdaJobs[count++] = lambdaJob;
        }
        m_ActiveJobs.clear();
        JobHandle cleanupJobHandle = JobSystem.ScheduleBatchJobs(lambdaJobs.data(), lambdaJobs.size());

        JobSystem::WaitForJob(cleanupJobHandle);

        for (auto& [id, shaderModule] : m_IdToShaderModuleMap)
        {
            if (shaderModule != VK_NULL_HANDLE && m_Device != VK_NULL_HANDLE)
                vkDestroyShaderModule(m_Device, shaderModule, nullptr);
        }
    }

    inline JobHandle VkShaderCache::CreateShaderModuleAsync(const IHashString* pShaderName, const VkShaderModuleCreateInfo &shaderModuleCreateInfo)
    {
        SAVANNA_ASSERT(pShaderName != nullptr, "Shader name is null!");

        if (m_IdToShaderModuleMap.find(pShaderName->GetHash()) != m_IdToShaderModuleMap.end())
        {
            SAVANNA_LOG_WARNING("Shader module with name {} already exists!", pShaderName);
            return k_DefaultJobHandle;
        }

        VkShaderModuleCreationJob shaderModuleCreationJob;
        shaderModuleCreationJob.m_pShaderCache = this;
        shaderModuleCreationJob.m_Device = m_Device;
        shaderModuleCreationJob.m_ShaderModuleCreateInfo = shaderModuleCreateInfo;
        shaderModuleCreationJob.m_pOutShaderModule = new VkShaderModule;

        JobHandle jobHandle = JobSystem::ScheduleJob(&shaderModuleCreationJob, JobPriority::k_Normal);

        std::lock_guard<std::mutex> lock(m_IdToShaderModuleMapMutex);
        m_IdToShaderModuleMap[pShaderName->GetHash()] = VK_NULL_HANDLE;
        m_ActiveJobs[jobHandle] = shaderModuleCreationJob;
    }
} // namespace Savanna::Gfx::Vk
