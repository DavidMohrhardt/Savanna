#include "ComputationalGeometry.h"

#include <Concurrency/JobSystem.h>

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

        // Read shaders from disk
        PrimitiveJob shaderCreateJobs[2]
        {
            PrimitiveJob{shaderJobFunc, &inputData[0]},
            PrimitiveJob{shaderJobFunc, &inputData[1]}
        };

        JobHandle shaderCreateJobHandles[2]
        {
            JobSystem::Get()->ScheduleJob(&shaderCreateJobs[0]),
            JobSystem::Get()->ScheduleJob(&shaderCreateJobs[1])
        };
        for (auto& handle : shaderCreateJobHandles)
        {
            JobSystem::Get()->AwaitCompletion(handle);
        }

        // JobSystem::Get()->AwaitCompletion(JobSystem::Get()->ScheduleJobBatch(shaderCreateJobPtrs, 2));
    }

} // namespace Savanna::Gfx::Vk
