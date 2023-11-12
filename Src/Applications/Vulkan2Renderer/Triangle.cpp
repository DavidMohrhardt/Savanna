#include "Triangle.h"

#include <Concurrency/Public/ISavannaJobs.hpp>

#include <Concurrency/ThreadManager.h>
#include <Concurrency/JobSystem.h>

#include <IO/VirtualFileSystem.h>
#include <IO/FileStream.h>

using namespace Savanna;
using namespace Savanna::Concurrency;

const char* k_DefaultTriangleShaderPaths[2]
{
    "Assets/SPIRV/SimpleTriangle.vert.spv",
    "Assets/SPIRV/SimpleTriangle.frag.spv"
};

constexpr float k_TriangleVertices[9]
{
    -0.5f, -0.5f, 0.0f, // bottom left
    0.5f, -0.5f, 0.0f, // bottom right
    0.0f, 0.5f, 0.0f // top
};

constexpr uint32_t k_TriangleIndices[3]
{
    0, 1, 2
};

constexpr uint32_t k_TriangleVertexCount = 3;
constexpr uint32_t k_TriangleIndexCount = 3;

constexpr uint32_t k_TriangleUvCoords[6]
{
    0, 0,
    1, 0,
    0, 1
};
constexpr uint32_t k_TriangleUvCoordCount = 6;

se_GfxShaderHandle_t TriangleMesh::s_DefaultTriangleShaderHandles[2]
{
    k_SavannaGfxInvalidShaderModuleHandle,
    k_SavannaGfxInvalidShaderModuleHandle
};

struct ShaderCreateJobInput
{
    const char* m_ShaderPath;
    se_GfxShaderCreateInfo_t m_CreateInfo;
    se_GfxShaderHandle_t* m_pShaderHandle;
};

class TriangleShaderCreateJob final : public IJob
{
private:
    ShaderCreateJobInput m_Input;

    void Dispose() { SAVANNA_DELETE(kSavannaAllocatorKindGeneral, this); }

public:
    TriangleShaderCreateJob(ShaderCreateJobInput& input)
        : m_Input{input}
    {}

    ~TriangleShaderCreateJob() = default;

    JobResult Execute() override
    {
        using namespace IO;
        SAVANNA_INSERT_SCOPED_PROFILER(TriangleShaderCreateJob::Execute);

        auto& shaderPath = m_Input.m_ShaderPath;
        auto& shaderCreateInfo = m_Input.m_CreateInfo;
        se_GfxShaderHandle_t* pOutShaderHandle = m_Input.m_pShaderHandle;

        try
        {
            // Get full path to shader
            FileStream stream(IO::VirtualFileSystem::Get()->GetFullPath(shaderPath));
            dynamic_array<se_byte> shaderBytes(std::move(stream.ReadFile<se_byte>(kSavannaAllocatorKindThreadSafeTemp)));

            shaderCreateInfo.m_pData = shaderBytes.data();
            shaderCreateInfo.m_Size = shaderBytes.size();

            // Create shader module
            return SAVANNA_GFX_SUCCESS(SavannaGfxCreateShaderModule(shaderCreateInfo, *pOutShaderHandle))
                ? JobResult::k_SavannaJobResultSuccess
                : JobResult::k_SavannaJobResultError;
        }
        catch(const std::exception& e)
        {
            SAVANNA_FATAL_LOG("Failed to create shader module: {}", e.what());
            return JobResult::k_SavannaJobResultError;
        }
    }

    protected:
        void OnComplete() override { Dispose(); }

        void OnCancel() override { Dispose(); }

        void OnError() override { Dispose(); }
};

inline TriangleShaderCreateJob* TriangleMesh::CreateShaderJob(
    se_uint8 index,
    se_GfxShaderStage_t stage,
    const se_AllocatorInterface_t* pAllocatorInterface)
{
    ShaderCreateJobInput input {
        k_DefaultTriangleShaderPaths[index],
        {},
        &s_DefaultTriangleShaderHandles[index]
    };

    input.m_CreateInfo.m_Stage = stage;

    return SAVANNA_NEW(kSavannaAllocatorKindGeneral, TriangleShaderCreateJob, input);
}

bool TriangleMesh::TryCreateDefaultShader(const se_AllocatorInterface_t* pAllocatorInterface)
{
    static auto didCreateShaders = [=]() -> bool
    {
        using namespace Concurrency;
        SAVANNA_INSERT_SCOPED_PROFILER(VulkanApplication::CreateShaderModules);

        JobHandle shaderJobHandles[2]
        {
            k_InvalidJobHandle,
            k_InvalidJobHandle
        };

        TriangleShaderCreateJob* jobs[2]
        {
            CreateShaderJob(0, kSavannaGfxShaderStageVertex, pAllocatorInterface),
            CreateShaderJob(1, kSavannaGfxShaderStageVertex, pAllocatorInterface)
        };

        JobHandle handles[2]
        {
            ThreadManager::Get()->GetJobSystem()->ScheduleJob(jobs[0]),
            ThreadManager::Get()->GetJobSystem()->ScheduleJob(jobs[1])
        };

        // Read shaders from disk
        for (int i = 0; i < 2; ++i)
        {
            ThreadManager::Get()->GetJobSystem()->AwaitCompletion(handles[i]);
            auto& shaderHandle = s_DefaultTriangleShaderHandles[i];
            if (shaderHandle == k_SavannaGfxInvalidShaderModuleHandle)
            {
                SAVANNA_FATAL_LOG("Failed to create shader module.");
                return false;
            }
        }

        return true;
    }();
    return didCreateShaders;
}

TriangleMesh &TriangleMesh::GetDefaultMesh()
{
    static TriangleMesh defaultMesh = []()->TriangleMesh
    {
        if (TryCreateDefaultShader(Savanna::MemoryManager::GetAllocatorInterfaceForAllocatorKindPtr(kSavannaAllocatorKindTemp)))
        {
            SAVANNA_LOG("Successfully created default triangle shader.");
        }
        else
        {
            SAVANNA_FATAL_LOG("Failed to create default triangle shader.");
        }

        return TriangleMesh
        {
            .m_pVertices = const_cast<float*>(k_TriangleVertices),
            .m_pIndices = const_cast<uint32_t*>(k_TriangleIndices),
            .m_pUvCoords = const_cast<uint32_t*>(k_TriangleUvCoords),
            .m_VertexCount = k_TriangleVertexCount,
            .m_IndexCount = k_TriangleIndexCount,
            .m_UvCoordCount = k_TriangleUvCoordCount
        };
    }();
    return defaultMesh;
}
