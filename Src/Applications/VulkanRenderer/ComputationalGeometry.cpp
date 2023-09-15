#include "ComputationalGeometry.h"

#include <Concurrency/JobSystem/JobManager.h>

#include <IO/FileStream.h>
#include <IO/VirtualFileSystem.h>

namespace Savanna::Gfx::Vk
{
    const char* k_DefaultShaderPaths[] = {
        "Assets/SPIRV/SimpleTriangle.vert.spv",
        "Assets/SPIRV/SimpleTriangle.frag.spv"
    };

    const char* k_ShaderNames[] = {
        "SimpleTriangleVertex",
        "SimpleTriangleFragment"
    };

    void Shapes::CreateDefaultShaderModules(Renderer& renderer)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(VulkanApplication::CreateShaderModules);

        struct ShaderCreateJobInput
        {
            const char* m_ShaderPath;
            FixedString64 m_ShaderName;
            Renderer& m_Renderer;
        };

        ShaderCreateJobInput inputData[2]
        {
            ShaderCreateJobInput{k_DefaultShaderPaths[0], k_ShaderNames[0], renderer},
            ShaderCreateJobInput{k_DefaultShaderPaths[1], k_ShaderNames[1], renderer}
        };

        JobExecuteFunc shaderJobFunc = [](void* pUserData) -> JobResult
        {
            SAVANNA_INSERT_SCOPED_PROFILER(VulkanApplication::CreateShaderModules::ShaderCreateJob);
            if (pUserData == nullptr)
            {
                SAVANNA_FATAL_LOG("Shader job data is null!");
                return JobResult::k_SavannaJobResultError;
            }

            ShaderCreateJobInput* pShaderCreateJobInput = reinterpret_cast<ShaderCreateJobInput*>(pUserData);

            auto& shaderPath = pShaderCreateJobInput->m_ShaderPath;
            auto& shaderName = pShaderCreateJobInput->m_ShaderName;
            auto& renderer = pShaderCreateJobInput->m_Renderer;

            try
            {
                // Get full path to shader
                IO::FileStream stream(IO::VirtualFileSystem::Get()->GetFullPath(shaderPath));
                std::vector<uint32_t> shaderBytes = stream.ReadFile<uint32_t>();

                // Create shader module
                return renderer.GetShaderCache().TryCreateShader(shaderName, renderer.GetGfxDevice(), shaderBytes)
                    ? JobResult::k_SavannaJobResultSuccess
                    : JobResult::k_SavannaJobResultError;
            }
            catch(const std::exception& e)
            {
                SAVANNA_FATAL_LOG("Failed to create shader module: {}", e.what());
                return JobResult::k_SavannaJobResultError;
            }
        };

        JobResultCallbackFunc shaderJobOnCompleteFunc = [](void*)
        {
            SAVANNA_LOG("Shader job completed!");
        };

        // Read shaders from disk
        PrimitiveJob shaderCreateJobs[2]
        {
            PrimitiveJob{shaderJobFunc, shaderJobOnCompleteFunc, &inputData[0]},
            PrimitiveJob{shaderJobFunc, shaderJobOnCompleteFunc, &inputData[1]}
        };

        // JobHandle shaderJobHandles[2]
        // {
        //     JobManager::Get().ScheduleJob(&shaderCreateJobs[0], JobPriority::k_SavannaJobPriorityHigh),
        //     JobManager::Get().ScheduleJob(&shaderCreateJobs[1], JobPriority::k_SavannaJobPriorityHigh)
        // };

        // for (size_t i = 0; i < 2; i++)
        // {
        //     JobManager::Get().AwaitCompletion(shaderJobHandles[i]);
        // }

        IJob* shaderCreateJobPtrs[2]
        {
            &shaderCreateJobs[0],
            &shaderCreateJobs[1]
        };

        for (size_t i = 0; i < 2; i++)
        {
            // JobManager::Get().ScheduleJob(shaderCreateJobPtrs[i], JobPriority::k_SavannaJobPriorityHigh);
            printf("Scheduling job with ptr: %p\n", shaderCreateJobPtrs[i]);
        }

        JobManager::Get().AwaitCompletion(JobManager::Get().ScheduleJobBatch(shaderCreateJobPtrs, 2, JobPriority::k_SavannaJobPriorityHigh));
    }

//     void Triangle::CreatePipelineStages()
//     {
//         VkPipelineShaderStageCreateInfo simpleTriangleInfos[2]
//         {
//             VkPipelineShaderStageCreateInfo{},
//             VkPipelineShaderStageCreateInfo{}
//         };

//         simpleTriangleInfos[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//         simpleTriangleInfos[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
//         if (!m_Renderer.GetShaderCache().TryGetShaderModule(k_ShaderNames[0], simpleTriangleInfos[0].module))
//         {
//             SAVANNA_FATAL_LOG("Failed to get shader module!");
//             return;
//         }
//         simpleTriangleInfos[0].pName = "main";

//         simpleTriangleInfos[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//         simpleTriangleInfos[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
//         if (!m_Renderer.GetShaderCache().TryGetShaderModule(k_ShaderNames[1], simpleTriangleInfos[1].module))
//         {
//             SAVANNA_FATAL_LOG("Failed to get shader module!");
//             return;
//         }
//         simpleTriangleInfos[1].pName = "main";

//         // Opt for dynamic viewport/scissor state
//         VkDynamicState dynamicStates[2]
//         {
//             VK_DYNAMIC_STATE_VIEWPORT,
//             VK_DYNAMIC_STATE_SCISSOR
//         };

//         VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo {};
//         dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
//         dynamicStateCreateInfo.dynamicStateCount = 2;
//         dynamicStateCreateInfo.pDynamicStates = dynamicStates;

//         VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo {};
//         vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
//         vertexInputStateCreateInfo.vertexBindingDescriptionCount = 0;
//         vertexInputStateCreateInfo.vertexAttributeDescriptionCount = 0;

//         VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo {};
//         inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
//         inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
//         inputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

//         VkViewport viewport = m_Renderer.GetSwapchain().GetDefaultViewportForSwapchain();
//         VkRect2D scissorRect {};
//         scissorRect.offset = {0, 0};
//         scissorRect.extent = m_Renderer.GetSwapchain().GetExtent();

//         VkPipelineViewportStateCreateInfo viewportStateCreateInfo {};
//         viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
//         viewportStateCreateInfo.viewportCount = 1;
//         viewportStateCreateInfo.pViewports = &viewport;
//         viewportStateCreateInfo.scissorCount = 1;
//         viewportStateCreateInfo.pScissors = &scissorRect;

//         VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo {};
//         rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
//         rasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
//         rasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
//         rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;

//         rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
//         rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;

//         rasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
//         rasterizationStateCreateInfo.lineWidth = 1.0f;

//         VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo {};
//         multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
//         multisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
//         multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

//         VkPipelineColorBlendAttachmentState colorBlendAttachmentState {};
//         colorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
//         colorBlendAttachmentState.blendEnable = VK_FALSE;

//         VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo {};
//         colorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
//         colorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
//         colorBlendStateCreateInfo.attachmentCount = 1;
//         colorBlendStateCreateInfo.pAttachments = &colorBlendAttachmentState;

//         VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo {};
//         pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
//         pipelineLayoutCreateInfo.setLayoutCount = 0;
//         pipelineLayoutCreateInfo.pushConstantRangeCount = 0;

//         // VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
//         // VkResult result = vkCreatePipelineLayout(m_Renderer.GetGfxDevice().GetDevice(), &pipelineLayoutCreateInfo, nullptr, &pipelineLayout);
//         // if (result != VK_SUCCESS)
//         // {
//         //     SAVANNA_FATAL_LOG("Failed to create pipeline layout: {}", VkResultToString(result));
//         //     return;
//         // }
//     }

} // namespace Savanna::Gfx::Vk

// void Savanna::Gfx::Vk::Shapes::CreateDefaultShaderModules(Renderer & renderer)
// {
// }
