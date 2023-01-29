/**
 * @file VkShaderCreateJob.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-01-27
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
    struct VkShaderModuleCreationJob : public IJob
    {
        VkShaderModuleCreateInfo m_ShaderModuleCreateInfo;
        VkShaderModule m_OutShaderModule;

        virtual bool Execute(void* pData = nullptr) override;
    };
} // namespace Savanna::Gfx::Vk
