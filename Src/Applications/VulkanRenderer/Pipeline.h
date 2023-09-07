#pragma once

#include "Types/Enums/Flags.h"

#include <vulkan/vulkan.h>

namespace Savanna::Gfx::Vk
{
    class Pipeline
    {
    private:
        VkPipeline m_Pipeline = VK_NULL_HANDLE;
        VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;

    public:
        Pipeline() = default;
        ~Pipeline() = default;

        Pipeline(const Pipeline&) = delete;
        Pipeline& operator=(const Pipeline&) = delete;

        Pipeline(Pipeline&& other) noexcept
            : m_Pipeline(std::exchange(other.m_Pipeline, VK_NULL_HANDLE))
            , m_PipelineLayout(std::exchange(other.m_PipelineLayout, VK_NULL_HANDLE))
        {
        }

        Pipeline& operator=(Pipeline&& other) noexcept
        {
            if (this != &other)
            {
                m_Pipeline = std::exchange(other.m_Pipeline, VK_NULL_HANDLE);
                m_PipelineLayout = std::exchange(other.m_PipelineLayout, VK_NULL_HANDLE);
            }
            return *this;
        }

        void Create(const VkDevice& device, const VkPipelineCache& pipelineCache, const VkGraphicsPipelineCreateInfo& createInfo);
        void Destroy(const VkDevice& device);

        inline VkPipeline GetPipeline() const { return m_Pipeline; }
        inline VkPipelineLayout GetPipelineLayout() const { return m_PipelineLayout; }

        inline bool IsValid() const { return m_Pipeline != VK_NULL_HANDLE; }

        inline void Bind(const VkCommandBuffer commandBuffer, const VkPipelineBindPoint bindPoint) const
        {
            vkCmdBindPipeline(commandBuffer, bindPoint, m_Pipeline);
        }

        VkPipeline& operator*() { return m_Pipeline; }
        const VkPipeline& operator*() const { return m_Pipeline; }
    };
} // namespace Savanna
