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
        VkDevice m_Device;
        std::map<const HashString, VkShaderModule> m_IdToShaderModuleMap;

    public:
        VkShaderCache(const VkDevice& device) SAVANNA_NO_EXCEPT;
        ~VkShaderCache();

    public:
        JobHandle CreateShaderModuleAsync(const HashString* pShaderName, const VkShaderModuleCreateInfo& shaderModuleCreateInfo);
    };

    VkShaderCache::VkShaderCache(const VkDevice& device) SAVANNA_NO_EXCEPT
        : m_Device(device)
        , m_IdToShaderModuleMap()
    {
    }

    VkShaderCache::~VkShaderCache()
    {
        for (auto& [id, shaderModule] : m_IdToShaderModuleMap)
        {
            if (shaderModule != VK_NULL_HANDLE && m_Device != VK_NULL_HANDLE)
                vkDestroyShaderModule(m_Device, shaderModule, nullptr);
        }
    }

    inline JobHandle VkShaderCache::CreateShaderModuleAsync(const HashString *pShaderName, const VkShaderModuleCreateInfo &shaderModuleCreateInfo)
    {
        SAVANNA_ASSERT(pShaderName != nullptr, "Shader name is null!");

        if (m_IdToShaderModuleMap.find(pShaderName->GetHash()) != m_IdToShaderModuleMap.end())
        {
            SAVANNA_LOG_WARNING("Shader module with name {} already exists!", pShaderName->GetCString());
            return k_DefaultJobHandle;
        }
    }

    JobHandle VkShaderCache::CreateShaderModuleAsync(const VkShaderModuleCreateInfo& shaderModuleCreateInfo)
    {
        JobHandle handle = k_DefaultJobHandle;
        if ()
        // VkShaderModuleCreationJob* shaderModuleCreationJob = new VkShaderModuleCreationJob();
        // shaderModuleCreationJob->m_Device = m_Device;
        // shaderModuleCreationJob->m_ShaderModuleCreateInfo = shaderModuleCreateInfo;

        // JobHandle shaderModuleCreationJobHandle = JobSystem::GetInstance().AddJob(shaderModuleCreationJob);

        // return shaderModuleCreationJobHandle;
    }
} // namespace Savanna::Gfx::Vk
