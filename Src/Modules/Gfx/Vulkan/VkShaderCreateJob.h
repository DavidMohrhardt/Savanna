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
    using namespace Savanna::Concurrency;

    struct VkShaderModuleCreationJob : public IJob
    {
    public:
        VkDevice m_Device;
        VkShaderModuleCreateInfo m_ShaderModuleCreateInfo;

    private:
        VkShaderModule m_OutShaderModule;

    public:
        virtual bool Execute() override;
    };
} // namespace Savanna::Gfx::Vk
