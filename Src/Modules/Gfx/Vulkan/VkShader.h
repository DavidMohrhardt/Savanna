#pragma once

#include <SavannaVk.h>

#include <Utilities/SavannaCoding.h>
#include <Public/ISavannaJobs.h>

namespace Savanna::Gfx::Vk
{
    using namespace Savanna::Concurrency;
    struct ShaderModuleCreationJob : public IJob
    {
        friend class Shader;

    public:
        const ShaderCache* m_pShaderCache;
        VkDevice m_Device;
        VkShaderModuleCreateInfo m_ShaderModuleCreateInfo;
        VkShaderModule* m_pOutShaderModule;

    public:
        VkShaderModuleCreationJob(const VkDevice& device, const VkShaderModuleCreateInfo& shaderModuleCreateInfo, VkShaderModule* pOutShaderModule) SAVANNA_NOEXCEPT
            : m_Device(device)
            , m_ShaderModuleCreateInfo(shaderModuleCreateInfo)
            , m_pOutShaderModule(pOutShaderModule)
        {
        }

    public:
        virtual bool Execute() SAVANNA_OVERRIDE
        {
            SAVANNA_ASSERT(m_pShaderCache != nullptr, "Shader cache is null!");
            SAVANNA_ASSERT(m_Device != VK_NULL_HANDLE, "Device is null!");
            SAVANNA_ASSERT(m_pOutShaderModule != nullptr, "Output shader module is null!");

            VkShaderModule shaderModule;
            VkResult result = vkCreateShaderModule(m_Device, &m_ShaderModuleCreateInfo, nullptr, &shaderModule);
            if (result != VK_SUCCESS)
            {
                SAVANNA_LOG_ERROR("Failed to create shader module! Error code: {}", result);
                return false;
            }

            *m_pOutShaderModule = shaderModule;

            return true;
        }
    };

    class ShaderCache
    {
    private:
        using ShaderModuleMap = std::unordered_map<CompileTimeHashString, VkShaderModule>;
        ShaderModuleMap m_ShaderModuleMap;

    public:
        ShaderCache() = default;
        ShaderCache(const ShaderCache&) = delete;
        ShaderCache(ShaderCache&& other) SAVANNA_NOEXCEPT;
        ~ShaderCache();

        ShaderCache& operator=(const ShaderCache&) = delete;
        ShaderCache& operator=(ShaderCache&& other) SAVANNA_NOEXCEPT;

        bool IsValid() const;

        void Create(const VkDevice& device);
        void Destroy();

        VkShaderModule GetShaderModule(const CompileTimeHashString& shaderName);
    };

    class Shader
    {
    private:
        VkShaderModule m_ShaderModule = VK_NULL_HANDLE;

    public:
        Shader() = default;
        Shader(const Shader&) = delete;
        Shader(Shader&& other) noexcept;
        ~Shader();

        Shader& operator=(const Shader&) = delete;
        Shader& operator=(Shader&& other) noexcept;

        bool IsValid() const;

        void Create(const VkDevice& device, const VkShaderModuleCreateInfo& shaderModuleCreateInfo);
        void Destroy();
    };
} // namespace Savanna::Gfx::Vk
