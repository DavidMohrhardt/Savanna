/**
 * @file VkShaderModuleCache.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
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

#include "Types/Locks/MultiReadSingleWriteLock.h"

#include <unordered_map>

namespace savanna::gfx::vk
{
    using namespace concurrency;
    class ShaderModuleCache
    {
    private:
        friend class ShaderModuleCreateJob;
        friend class VkDriver;

        std::atomic_int64_t m_NextHandle = 1;

        std::unordered_map<seGfxShader, VkShaderModule> m_ShaderModules;
        MultiReadSingleWriteLock m_ReadWriteLock;

        JobHandle m_AllActiveShaderModulesJob = k_InvalidJobHandle;

        void RegisterShaderIdInternal(
            const seGfxShader& shaderModuleHandle,
            const VkShaderModule& shaderModule);

        void UnregisterShaderIdInternal(
            const seGfxShader& shaderModuleHandle);

        ShaderModuleCache() = default;
        ~ShaderModuleCache() = default;

        JobHandle CreateShaderModulesAsync(
            const VkGpu &gpu,
            const seGfxShaderCreateInfo *createInfos,
            const size_t createInfoCount,
            seGfxShader **const ppOutShaderModuleHandles);

        seGfxShader CreateShaderModuleSynchronized(
            const VkGpu& gpu,
            const seGfxShaderCreateInfo& createInfo);

        void DestroyShaderModules(
            const VkGpu& gpu,
            const seGfxShader* shaderModuleHandles,
            size_t shaderModuleHandleCount);

        void Clear(const VkGpu& gpu);

        void GetShaderModule(
            const VkGpu& gpu,
            const seGfxShader& shaderModuleHandle,
            VkShaderModule& outShaderModule);
    };
} // namespace savanna::Gfx::vk
