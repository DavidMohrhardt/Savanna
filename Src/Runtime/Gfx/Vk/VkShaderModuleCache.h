/**
 * @file VkShaderModuleCache.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-10-26
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "SavannaVk2.h"

#include "VkGpu.h"

// Grab the ISavannaJobs header to submit jobs to the job system.
#include "Concurrency/Public/ISavannaJobs.hpp"

#include "Types/Strings/FixedString.h"

#include <unordered_map>

namespace Savanna::Gfx::Vk2
{
    using namespace Concurrency;
    class VkShaderModuleCache
    {
    private:
        friend class VkShaderModuleCreateJob;
        friend class VkDriver;

        std::atomic_int64_t m_NextHandle = 0;

        std::unordered_map<se_GfxShaderModuleHandle_t, VkShaderModule> m_ShaderModules;
        AtomicCounter m_ReaderCounter;
        SpinLock m_WriterLock;

        JobHandle m_AllActiveShaderModulesJob = k_InvalidJobHandle;

        void RegisterShaderIdInternal(
            const se_GfxShaderModuleHandle_t& shaderModuleHandle,
            const VkShaderModule& shaderModule);

        void UnregisterShaderIdInternal(
            const se_GfxShaderModuleHandle_t& shaderModuleHandle);

        VkShaderModuleCache() = default;
        ~VkShaderModuleCache() = default;

        JobHandle CreateShaderModulesAsync(
            const VkGpu &gpu,
            const se_GfxShaderModuleCreateInfo_t *createInfos,
            const size_t createInfoCount,
            se_GfxShaderModuleHandle_t **const ppOutShaderModuleHandles);

        se_GfxShaderModuleHandle_t CreateShaderModuleSynchronized(
            const VkGpu& gpu,
            const se_GfxShaderModuleCreateInfo_t& createInfo);

        void DestroyShaderModules(
            const VkGpu& gpu,
            const se_GfxShaderModuleHandle_t* shaderModuleHandles,
            size_t shaderModuleHandleCount);

        void Clear(const VkGpu& gpu);

        void GetShaderModule(
            const VkGpu& gpu,
            const se_GfxShaderModuleHandle_t& shaderModuleHandle,
            VkShaderModule& outShaderModule);
    };
} // namespace Savanna::Gfx::Vk2
