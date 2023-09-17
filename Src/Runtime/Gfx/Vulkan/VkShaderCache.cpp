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
    ShaderCache::~ShaderCache()
    {
        Clear();
    }

    bool ShaderCache::TryCreateShader(
        const FixedString64 &shaderName,
        const VkDevice &device,
        std::vector<uint32_t>& shaderBinary,
        VkShaderStageFlagBits shaderStageType)
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

        //TODO @JWB Doesn't need allocation..?
        SE_VkShaderModule se_shaderModule{};
        se_shaderModule.m_Module = shaderModule;
        se_shaderModule.m_type = shaderStageType;

        RegisterShaderModule(shaderName, se_shaderModule);
        return true;
    }

    bool ShaderCache::TryGetShaderModule(const FixedString64 &shaderName, 
                                        SE_VkShaderModule &outShaderModule)
    {
        std::lock_guard<std::mutex> lock(m_DataMutex);
        if (m_ShaderModuleMap.find(shaderName) != m_ShaderModuleMap.end())
        {
            outShaderModule = m_ShaderModuleMap[shaderName];
            return true;
        }
        return false;
    }

    DynamicArray<SE_VkShaderModule> ShaderCache::GetShaderModules() 
    { 
        std::lock_guard<std::mutex> lock(m_DataMutex);
        DynamicArray<SE_VkShaderModule> values(m_ShaderModuleMap.size());
        
        for (auto kv : m_ShaderModuleMap) 
        {
            values.push_back(kv.second);
        } 

        return values;
    }

    void ShaderCache::Clear()
    {
        std::lock_guard<std::mutex> lock(m_DataMutex);
        for (auto& shaderModule : m_ShaderModuleMap)
        {
            vkDestroyShaderModule(m_Device, shaderModule.second.m_Module, nullptr); 
        }

        m_ShaderModuleMap.clear();
    }

    void ShaderCache::RegisterShaderModule(const FixedString64 &shaderName,
                                           SE_VkShaderModule shaderModule)
    {
        std::lock_guard<std::mutex> lock(m_DataMutex);
        m_ShaderModuleMap[shaderName] = shaderModule;
    }
} // namespace Savanna::Gfx::Vk
