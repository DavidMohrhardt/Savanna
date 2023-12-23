#include "Pipeline.h"

#include <stdexcept>

namespace savanna::Gfx::Vk
{
    void Pipeline::Create(
        const VkDevice& device,
        const VkPipelineCache& pipelineCache,
        const VkGraphicsPipelineCreateInfo &createInfo)
    {
        VkResult result = vkCreateGraphicsPipelines(device, pipelineCache, 1, &createInfo, nullptr, &m_Pipeline);
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create graphics pipeline!");
        }
    }

    void Pipeline::Destroy(const VkDevice &device)
    {
        vkDestroyPipeline(device, m_Pipeline, nullptr);
        vkDestroyPipelineLayout(device, m_PipelineLayout, nullptr);
    }

} // namespace savanna::Gfx::Vk
