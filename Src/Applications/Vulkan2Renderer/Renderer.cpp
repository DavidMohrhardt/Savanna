#include "Renderer.h"

#include <Gfx/Vk/Public/ISavannaGfxVk2.h>

#include <Types/Containers/Arrays/dynamic_array.h>

#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <Concurrency/JobManager.h>
#include <IO/FileStream.h>
#include <IO/VirtualFileSystem.h>

namespace vk
{
    using namespace Concurrency;

    // bool TryCreateShaders(fixed_array<se_GfxShaderHandle_t, 2>& outShaderHandles, se_AllocatorInterface_t* pAllocatorInterface)
    bool TryCreateShaders(se_GfxShaderHandle_t* ppOutShaderHandles, const se_AllocatorInterface_t* pAllocatorInterface)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(VulkanApplication::CreateShaderModules);
        const char* k_DefaultShaderPaths[2]
        {
            "Assets/SPIRV/SimpleTriangle.vert.spv",
            "Assets/SPIRV/SimpleTriangle.frag.spv"
        };

        struct ShaderCreateJobInput
        {
            const char* m_ShaderPath;
            se_GfxShaderCreateInfo_t m_CreateInfo;
            se_GfxShaderHandle_t* m_pShaderHandle;
            const se_AllocatorInterface_t* m_pAllocatorInterface;
        };

        ShaderCreateJobInput inputData[2]
        {
            ShaderCreateJobInput{k_DefaultShaderPaths[0], {}, &ppOutShaderHandles[0], pAllocatorInterface},
            ShaderCreateJobInput{k_DefaultShaderPaths[1], {}, &ppOutShaderHandles[1], pAllocatorInterface}
        };

        auto& vertexCreateInfo = inputData[0].m_CreateInfo;
        vertexCreateInfo.m_Stage = kSavannaGfxShaderStageVertex;
        vertexCreateInfo.m_pAllocatorInterface = pAllocatorInterface;

        auto& fragmentCreateInfo = inputData[1].m_CreateInfo;
        fragmentCreateInfo.m_Stage = kSavannaGfxShaderStageFragment;
        fragmentCreateInfo.m_pAllocatorInterface = pAllocatorInterface;

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
            auto& shaderCreateInfo = pShaderCreateJobInput->m_CreateInfo;
            se_GfxShaderHandle_t* pOutShaderHandle = pShaderCreateJobInput->m_pShaderHandle;

            try
            {
                // Get full path to shader
                IO::FileStream stream(IO::VirtualFileSystem::Get()->GetFullPath(shaderPath));
                std::vector<uint32_t> shaderBytes = stream.ReadFile<uint32_t>();

                shaderCreateInfo.m_pData = shaderBytes.data();
                shaderCreateInfo.m_Size = shaderBytes.size();

                // Create shader module
                // return renderer.GetShaderCache().TryCreateShader(shaderName, renderer.GetGfxDevice(), shaderBytes)
                return SAVANNA_GFX_SUCCESS(SavannaGfxCreateShaderModule(shaderCreateInfo, *pOutShaderHandle))
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
            JobManager::Get()->ScheduleJob(&shaderCreateJobs[0]),
            JobManager::Get()->ScheduleJob(&shaderCreateJobs[1])
        };

        JobManager::Get()->AwaitCompletion(shaderCreateJobHandles, 2);

        for (int i = 0; i < 2; ++i)
        {
            auto& shaderHandle = ppOutShaderHandles[i];
            if (shaderHandle == k_SavannaGfxInvalidShaderModuleHandle)
            {
                SAVANNA_FATAL_LOG("Failed to create shader module.");
                return false;
            }
        }

        return true;
    }

    Renderer::Renderer() {}

    Renderer::~Renderer()
    {
        SAVANNA_INSERT_SCOPED_PROFILER(Renderer::~Renderer);
    }

    bool Renderer::TryInitialize(const se_GfxContextCreateInfo_t* pCreateInfo, void* pWindowHandle)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(Application::TryInitGfx);
        auto pDefaultAllocInterface = Savanna::MemoryManager::GetAllocatorInterfaceForLabelPtr(k_SavannaMemoryLabelGfx);
        se_GfxContextCreateInfo_t gfxContextCreateInfo;
        if (pCreateInfo != nullptr)
        {
            gfxContextCreateInfo = *pCreateInfo;
        }
        else
        {
            gfxContextCreateInfo = {
                .m_pApplicationName = "SavannaVk2",
                .m_pAllocatorInterface = pDefaultAllocInterface,
                .m_pUserData = nullptr,
            };
        }

        if (SAVANNA_GFX_FAILURE(SavannaGfxInit(&gfxContextCreateInfo)))
        {
            SAVANNA_LOG("Failed to initialize graphics context.");
            return false;
        }

        uint32 instanceExtensionCount = 0;
        const char **glfwExtensions =
            glfwGetRequiredInstanceExtensions(&instanceExtensionCount);

        se_VkDriverCreateInfo_t vkDriverCreateInfo{};
        vkDriverCreateInfo.m_pNext = nullptr;
        vkDriverCreateInfo.m_pUserData = nullptr;
        vkDriverCreateInfo.m_EnableValidationLayers = true;
        vkDriverCreateInfo.m_RequestSurface = true;
        vkDriverCreateInfo.m_pWindowHandle =
        #if SAVANNA_WINDOWS
            glfwGetWin32Window((GLFWwindow*)pWindowHandle);
        #else
            nullptr;
        #endif

        vkDriverCreateInfo.m_InstanceCreateArgs.m_ppEnabledInstanceExtensions =
            glfwExtensions;
        vkDriverCreateInfo.m_InstanceCreateArgs.m_EnabledInstanceExtensionCount =
            instanceExtensionCount;

        vkDriverCreateInfo.m_PhysicalDeviceCreateArgs.m_PreferredGraphicsDeviceIndex =
            -1;

        dynamic_array<const char *> enabledDeviceExtensions{
            1, k_SavannaMemoryArenaIdGfx};
        if (vkDriverCreateInfo.m_RequestSurface) {
            enabledDeviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
        }

        vkDriverCreateInfo.m_LogicalDeviceCreateArgs.m_ppEnabledDeviceExtensions =
            enabledDeviceExtensions.data();
        vkDriverCreateInfo.m_LogicalDeviceCreateArgs.m_EnabledDeviceExtensionCount =
            enabledDeviceExtensions.size();

        se_GfxDriverCreateInfo_t gfxDriverCreateInfo{
            .m_RequestedBackendType = kSavannaGfxApiVulkan,
            .m_pAllocationInterface = pDefaultAllocInterface,
            .m_pNext = &vkDriverCreateInfo,
            .m_pUserData = nullptr,
        };

        se_GfxDriverCreateInfoList_t gfxDriverCreateInfoList{
            .m_pDriverCreateInfos = &gfxDriverCreateInfo,
            .m_CreateInfoCount = 1,
            .m_pUserData = nullptr,
        };

        if (SAVANNA_GFX_FAILURE(SavannaGfxContextCreateDriver(&gfxDriverCreateInfoList)))
        {
            SAVANNA_LOG("Failed to create graphics driver.");
            return false;
        }

        se_GfxSwapchainCreateInfo_t gfxSwapchainCreateInfo{};
        gfxSwapchainCreateInfo.m_ImageCount = 3;
        gfxSwapchainCreateInfo.m_Format = k_SavannaGfxTextureFormat_B8G8R8A8_UNORM;
        glfwGetWindowSize((GLFWwindow*)pWindowHandle,
                            (int *)&gfxSwapchainCreateInfo.m_Width,
                            (int *)&gfxSwapchainCreateInfo.m_Height);

        se_GfxHandle_t swapchainHandle;
        if (SAVANNA_GFX_FAILURE(SavannaGfxCreateSwapchain(&gfxSwapchainCreateInfo, &swapchainHandle)))
        {
            SAVANNA_LOG("Failed to request swapchain.");
            return false;
        }

        return TryCreateShaders(m_DefaultShaderHandles, pDefaultAllocInterface);
    }
} // namespace vk
