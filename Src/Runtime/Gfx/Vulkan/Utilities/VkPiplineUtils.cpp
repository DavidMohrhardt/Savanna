/*
 * @file 
 * @author 
 * @brief
 * @version 0.1
 * @date 
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "VkPiplineUtils.h"
//#include "Memory/MemoryManager.h"

#include <algorithm>

namespace Savanna::Gfx::Vk::Utils
{
    DynamicArray<VkPipelineShaderStageCreateInfo> CreateShaderStages(DynamicArray<SE_VkShaderModule>& modules) 
    {    
        DynamicArray<VkPipelineShaderStageCreateInfo> shaderStages(modules.size());

        //TODO @JWB Jobify
        //for (auto m : modules) {
        for (int i = 0; i < modules.size(); ++i)
        {  
            auto m = modules[i];

            VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
            vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            vertShaderStageInfo.stage = m.m_type;
            vertShaderStageInfo.module = m.m_Module;
            vertShaderStageInfo.pName = "main";
            //TODO @JWB learn about pSpecializationInfo

            shaderStages.push_back(vertShaderStageInfo);
        }


        return shaderStages;
    }

}