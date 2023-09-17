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
#include "Utilities/Semantics/Move.h"

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
        std::vector<uint32_t> m_ShaderBinary;

    public:
        ShaderModuleCreationJob(
            ShaderCache* pShaderCache,
            const FixedString64 &shaderName,
            const VkDevice& device,
            std::vector<uint32_t>& shaderBinary) SAVANNA_NOEXCEPT
            : m_pShaderCache(pShaderCache)
            , m_ShaderName(shaderName)
            , m_Device(device)
            , m_ShaderBinary(std::move(shaderBinary))
        {}

        // ShaderModuleCreationJob(ShaderModuleCreationJob &&) = delete;
        // TODO determine why this operator doesn't work as expected.
        // ShaderModuleCreationJob& operator=(ShaderModuleCreationJob&&) noexcept = default;

    public:
        virtual JobResult Execute() SAVANNA_OVERRIDE
        {
            SAVANNA_ASSERT(m_pShaderCache != nullptr, "Shader cache is null!");
            SAVANNA_ASSERT(m_Device != VK_NULL_HANDLE, "Device is null!");

            return m_pShaderCache->TryCreateShader(m_ShaderName, m_Device, m_ShaderBinary)
                ? k_SavannaJobResultSuccess
                : k_SavannaJobResultError;
        }
    };

    ShaderCache::~ShaderCache()
    {
        Clear();
    }

    bool ShaderCache::TryCreateShader(
        const FixedString64 &shaderName,
        const VkDevice &device,
        std::vector<uint32_t>& shaderBinary)
    {
        VkShaderModuleCreateInfo shaderModuleCreateInfo{};
        shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderModuleCreateInfo.codeSize = shaderBinary.size();
        shaderModuleCreateInfo.pCode = static_cast<const uint32_t*>(shaderBinary.data());
        shaderModuleCreateInfo.flags = 0;

        VkShaderModule shaderModule{};
        VkResult result = vkCreateShaderModule(device, &shaderModuleCreateInfo, nullptr, &shaderModule);
        if (result != VK_SUCCESS)
        {
            SAVANNA_WARNING_LOG("Failed to create shader module! Error code: {}!", ResultToString(result));
            return false;
        }

        RegisterShaderModule(shaderName, shaderModule);
        return true;
    }

    bool ShaderCache::TryGetShaderModule(const FixedString64 &shaderName, VkShaderModule &outShaderModule)
    {
        std::lock_guard<std::mutex> lock(m_DataMutex);
        if (m_ShaderModuleMap.find(shaderName) != m_ShaderModuleMap.end())
        {
            outShaderModule = m_ShaderModuleMap[shaderName];
            return true;
        }
        return false;
    }

    void ShaderCache::Clear()
    {
        std::lock_guard<std::mutex> lock(m_DataMutex);
        for (auto& shaderModule : m_ShaderModuleMap)
        {
            vkDestroyShaderModule(m_Device, shaderModule.second, nullptr);
        }

        m_ShaderModuleMap.clear();
    }

    void ShaderCache::RegisterShaderModule(const FixedString64& shaderName, VkShaderModule shaderModule)
    {
        std::lock_guard<std::mutex> lock(m_DataMutex);
        m_ShaderModuleMap[shaderName] = shaderModule;
    }
} // namespace Savanna::Gfx::Vk
