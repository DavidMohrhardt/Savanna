/**
 * @file VkShaderCache.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-05-18
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "SavannaEngine.h"
#include "Utilities/SavannaCoding.h"

#include <JobSystem/ISavannaJobs.h>

#include <vulkan/vulkan.h>

#include <Types/Strings/FixedString.h>
#include <Utilities/Console.h>

#include "VkGfxDevice.h"
#include "Types/Containers/Arrays/DynamicArray.h"

#include <mutex>
#include <unordered_map>

namespace Savanna::Gfx::Vk
{
    using namespace Savanna::Concurrency;

    struct SE_VkShaderModule {
      VkShaderModule m_Module;
      VkShaderStageFlagBits m_type; 
    };

    class ShaderCache
    {
    private:
        using ShaderModuleMap = std::unordered_map<FixedString64, SE_VkShaderModule>;

        VkDevice m_Device;
        ShaderModuleMap m_ShaderModuleMap;

        std::mutex m_DataMutex;

        friend class Renderer;
        //friend class ShaderModuleCreationJob;

    public:
        ShaderCache() = default;
        ShaderCache(const GfxDevice& gfxDevice)
            : m_Device(gfxDevice.GetGfxDevice())
        {
            m_ShaderModuleMap.reserve(8);
        }

        ~ShaderCache();

        ShaderCache(const ShaderCache&) = delete;

        ShaderCache(ShaderCache&& other) SAVANNA_NOEXCEPT
            : m_ShaderModuleMap(std::move(other.m_ShaderModuleMap))
            , m_Device(std::move(other.m_Device))
        {
            SAVANNA_MOVE_MEMBER(m_Device, other);
            SAVANNA_MOVE_MEMBER(m_ShaderModuleMap, other);
        }

        ShaderCache& operator=(const ShaderCache&) = delete;

        inline ShaderCache& operator=(ShaderCache&& other) SAVANNA_NOEXCEPT
        {
            SAVANNA_MOVE_MEMBER(m_Device, other);
            SAVANNA_MOVE_MEMBER(m_ShaderModuleMap, other);
            return *this;
        }

        bool TryCreateShader(
            const FixedString64& shaderName,
            const VkDevice& device,
            std::vector<uint32_t>& shaderBinary,
            VkShaderStageFlagBits shaderStageType);

        bool TryGetShaderModule(const FixedString64 &shaderName,
                                SE_VkShaderModule &outShaderModule);

        DynamicArray<SE_VkShaderModule> GetShaderModules();

    private:
        void Clear();

        void RegisterShaderModule(const FixedString64 &shaderName,
                                  SE_VkShaderModule shaderModule);
    };
} // namespace Savanna::Gfx::Vk
