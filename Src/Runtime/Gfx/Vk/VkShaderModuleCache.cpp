#include "VkShaderModuleCache.h"

#include "VkAllocator.h"

#include "Concurrency/AutoDisposeJob.h"
#include "Concurrency/ThreadManager.h"
#include "Concurrency/JobSystem.h"

// TODO @DavidMohrhardt: Move up a level. It's the GFX system's responsibility to manage the shader resources.

namespace savanna::gfx::vk
{
    inline VkResult CreateVkShaderModule(
        const VkGpu& gpu,
        const se_GfxShaderCreateInfo_t& createInfo,
        VkShaderModule& outShaderModule)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(CreateVkShaderModule);
        VkShaderModuleCreateInfo shaderModuleCreateInfo {};
        shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderModuleCreateInfo.codeSize = createInfo.m_Size;
        shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(createInfo.m_pData);

        return vkCreateShaderModule(gpu, &shaderModuleCreateInfo, VkAllocator::Get(), &outShaderModule);
    }

    using namespace concurrency;
    struct ShaderModuleCreateJob final : public AutoDisposeJobBase
    {
    private:
        DECLARE_SAVANNA_MEMORY_CLASS_FRIENDS();
        friend class ShaderModuleCache;

        void Dispose()
        {
            SAVANNA_INSERT_SCOPED_PROFILER(ShaderModuleCreateJob::Dispose);
            SAVANNA_DELETE(kSavannaAllocatorKindThreadSafeTemp, this);
        }

        ShaderModuleCache& m_ShaderModuleCache;
        const VkGpu& m_Gpu;

        dynamic_array<se_GfxShaderCreateInfo_t> m_CreateInfos;
        dynamic_array<se_GfxShaderHandle_t> m_ShaderModuleHandles;

        std::atomic_uint8_t m_State = Waiting;

        enum State : uint8
        {
            Waiting,
            Executing,
            Completed,
            Cancelled,
        };

        ShaderModuleCreateJob() = delete;

        ShaderModuleCreateJob(
            ShaderModuleCache& shaderModuleCache,
            const VkGpu& gpu)
            : AutoDisposeJobBase(kSavannaAllocatorKindThreadSafeTemp)
            , m_ShaderModuleCache(shaderModuleCache)
            , m_Gpu(gpu)
            , m_CreateInfos(1, kSavannaAllocatorKindThreadSafeTemp)
            , m_ShaderModuleHandles(1, kSavannaAllocatorKindThreadSafeTemp)
        {
        }

    public:
        ~ShaderModuleCreateJob() override = default;

        JobResult Execute() override
        {
            SAVANNA_INSERT_SCOPED_PROFILER(ShaderModuleCreateJob::Execute);
            uint8 expectedState = State::Waiting;
            if (!m_State.compare_exchange_strong(expectedState, State::Executing))
            {
                auto state = static_cast<State>(m_State.load(std::memory_order_relaxed));
                switch (expectedState)
                {
                case State::Cancelled:
                    return k_SavannaJobResultCancelled;
                default:
                    return k_SavannaJobResultError;
                }
            }

            for (int i = 0; i < m_CreateInfos.size(); ++i)
            {
                auto& createInfo = m_CreateInfos[i];

                VkShaderModule shaderModule;
                auto result = CreateVkShaderModule(m_Gpu, createInfo, shaderModule);
                if (result != VK_SUCCESS)
                {
                    SAVANNA_LOG("Failed to create shader module.");
                    m_ShaderModuleCache.UnregisterShaderIdInternal(m_ShaderModuleHandles[i]);
                }

                m_ShaderModuleCache.RegisterShaderIdInternal(m_ShaderModuleHandles[i], shaderModule);
            }

            return k_SavannaJobResultSuccess;
        }
    };

    inline void ShaderModuleCache::RegisterShaderIdInternal(
        const se_GfxShaderHandle_t& shaderModuleHandle,
        const VkShaderModule& shaderModule)
    {
        SAVANNA_WRITE_CRITICAL_SECTION(m_ReadWriteLock,
        {
            m_ShaderModules[shaderModuleHandle] = shaderModule;
        });
    }

    inline void ShaderModuleCache::UnregisterShaderIdInternal(
        const se_GfxShaderHandle_t& shaderModuleHandle)
    {
        SAVANNA_WRITE_CRITICAL_SECTION(m_ReadWriteLock,
        {
            m_ShaderModules.erase(shaderModuleHandle);
        });
    }

    JobHandle ShaderModuleCache::CreateShaderModulesAsync(
        const VkGpu &gpu,
        const se_GfxShaderCreateInfo_t *createInfos,
        const size_t createInfoCount,
        se_GfxShaderHandle_t **const ppOutShaderModuleHandles)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(ShaderModuleCache::CreateShaderModulesAsync);
        ThreadManager* pThreadManager = ThreadManager::Get();

        if (pThreadManager == nullptr || createInfoCount == 0 || createInfos == nullptr)
        {
            return k_InvalidJobHandle;
        }

        auto pJob = SAVANNA_NEW(kSavannaAllocatorKindThreadSafeTemp, ShaderModuleCreateJob, *this, gpu);

        // Critical section, registers the shader module handles before the job is scheduled.
        SAVANNA_WRITE_CRITICAL_SECTION(m_ReadWriteLock,
        {
            for (size_t i = 0; i < createInfoCount; ++i)
            {
                se_GfxShaderHandle_t handle = m_NextHandle.fetch_add(1, std::memory_order_relaxed);
                // Register the handle even though the shader module hasn't been created yet.
                m_ShaderModules[handle] = VK_NULL_HANDLE;
                pJob->m_CreateInfos.push_back(createInfos[i]);
                pJob->m_ShaderModuleHandles.push_back(handle);
            }
        });

        JobHandle jobHandle = pThreadManager->GetJobSystem()->ScheduleJob(static_cast<IJob*>(pJob), k_SavannaJobPriorityHigh);

        // From here we combine the dependencies of the job we're about to schedule with the dependencies of the jobs that are already running. This way we can wait for all of them to finish before we continue.
        JobHandle handlesArray[2] { jobHandle, m_AllActiveShaderModulesJob };
        m_AllActiveShaderModulesJob = pThreadManager->GetJobSystem()->CombineDependencies(handlesArray, 2);

        return jobHandle;
    }

    se_GfxShaderHandle_t ShaderModuleCache::CreateShaderModuleSynchronized(
        const VkGpu& gpu,
        const se_GfxShaderCreateInfo_t& createInfo)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(ShaderModuleCache::CreateShaderModule);
        VkShaderModule shaderModule = VK_NULL_HANDLE;
        auto result = CreateVkShaderModule(gpu, createInfo, shaderModule);
        if (result != VK_SUCCESS)
        {
            SAVANNA_LOG("Failed to create shader module.");
            return k_SavannaGfxInvalidShaderModuleHandle;
        }

        se_GfxShaderHandle_t handle = m_NextHandle.fetch_add(1, std::memory_order_relaxed);
        RegisterShaderIdInternal(handle, shaderModule);

        return handle;
    }

    void ShaderModuleCache::Clear(const VkGpu& gpu)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(ShaderModuleCache::Clear);
        SAVANNA_WRITE_CRITICAL_SECTION(m_ReadWriteLock,
        for (auto& [handle, shaderModule] : m_ShaderModules)
        {
            vkDestroyShaderModule(gpu, shaderModule, VkAllocator::Get());
        });
        m_ShaderModules.clear();
    }

    void ShaderModuleCache::GetShaderModule(
        const VkGpu &gpu,
        const se_GfxShaderHandle_t &shaderModuleHandle,
        VkShaderModule& outShaderModule)
    {
        SAVANNA_READ_CRITICAL_SECTION(m_ReadWriteLock,
        {
            auto it = m_ShaderModules.find(shaderModuleHandle);
            if (it == m_ShaderModules.end())
            {
                SAVANNA_LOG("Shader module not found.");
                return;
            }
            outShaderModule = it->second;
        });
    }
} // namespace savanna::Gfx::vk
