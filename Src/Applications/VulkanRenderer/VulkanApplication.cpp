/**
 * @file VulkanApplication.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 */

#include "VulkanApplication.h"

// Savanna Vulkan Includes
#include <VkSurfaceCreateInfos.h>
#include <Utilities/VkCallbacks.h>
#include <Utilities/VkLayerUtils.h>
#include <Utilities/VkExtensionUtils.h>
#include <Windows/WindowsVkDisplaySurface.h>

// Savanna Includes
#include <Profiling/Profiler.h>

#include <JobSystem/JobManager.h>

#include <VirtualFileSystem.h>

#include <FileStream.h>

#define GLFW_INCLUDE_VULKAN
#include <iostream>
#include <string.h>
#include <glm/glm.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

namespace Savanna::Application
{
    const char* k_ApplicationName = "Savanna";
    const char* k_EngineName = "No Engine";

    const char* k_DefaultShaderPaths[] = {
        "Assets/Shaders/SPIRV/SimpleTriangle.vert.spv",
        "Assets/Shaders/SPIRV/SimpleTriangle.frag.spv"
    };

    const char* k_ShaderNames[] = {
        "SimpleTriangleVertex",
        "SimpleTriangleFragment"
    };

    VulkanApplication::VulkanApplication()
        : m_Window(glfwCreateWindow(1920, 1080, "Savanna", nullptr, nullptr))
        , m_Renderer()
    {
        SAVANNA_INSERT_SCOPED_PROFILER(VulkanApplication::ctor);

        CreateRenderer();
        CreateShaderModules();
    }

    VulkanApplication::~VulkanApplication()
    {
        SAVANNA_INSERT_SCOPED_PROFILER(VulkanApplication::~VulkanApplication());

        Concurrency::JobManager::Get()->Stop(true);
        Concurrency::JobManager::Destroy();

        IO::VirtualFileSystem::Destroy();
    }

    void VulkanApplication::Run()
    {
        while(!m_Window.ShouldClose())
        {
            m_Window.PollEvents();
        }
    }

    void VulkanApplication::CreateRenderer()
    {
        using namespace Savanna;
        using namespace Savanna::Gfx::Vk;

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions {
#if SAVANNA_VULKAN_DEBUGGING
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif // ENABLE_VALIDATION_LAYERS
        };

        for (uint32_t i = 0; i < glfwExtensionCount; ++i)
        {
            extensions.push_back(glfwExtensions[i]);
        }

        std::vector<const char*> layers {
#if SAVANNA_VULKAN_DEBUGGING
            "VK_LAYER_KHRONOS_validation"
#endif // ENABLE_VALIDATION_LAYERS
        };

        const uint32_t deviceExtensionsCount = 1;

        RendererCreateInfo rendererCreateInfo {0};
        rendererCreateInfo.m_ApplicationName = k_ApplicationName;
        rendererCreateInfo.m_EngineName = k_EngineName;
        rendererCreateInfo.m_Width = 1920;
        rendererCreateInfo.m_Height = 1080;
        rendererCreateInfo.m_ppInstanceExtensions = extensions.data();
        rendererCreateInfo.m_InstanceExtensionsCount = static_cast<uint32>(extensions.size());
        rendererCreateInfo.m_ppEnabledLayerNames = layers.data();
        rendererCreateInfo.m_EnabledLayerCount = static_cast<uint32>(layers.size());

#if SAVANNA_WINDOWS
        Windows::FillOutSurfaceCreateInfo(GetModuleHandle(nullptr), glfwGetWin32Window(m_Window.GetWindowPtr()), &rendererCreateInfo.m_SurfaceCreateInfo);
#else
        #error "Unsupported platform!"
#endif

        // TODO @DavidMohrhardt Allow for additional extensions to be added via queries
        std::vector<const char*> deviceExtensions {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        };
        rendererCreateInfo.m_ppDeviceExtensions = deviceExtensions.data();
        rendererCreateInfo.m_DeviceExtensionsCount = static_cast<uint32>(deviceExtensions.size());

        rendererCreateInfo.m_QueueFlags = seVkQueueFlagBitsGraphicsBit | seVkQueueFlagBitsPresentBit;

        m_Renderer.Create(&rendererCreateInfo);
    }

    void VulkanApplication::CreateShaderModules()
    {
        Concurrency::JobManager::Construct((uint8)std::thread::hardware_concurrency());
        IO::VirtualFileSystem::Construct();

        Concurrency::JobManager::Get()->Start();

        auto& shaderCache = m_Renderer.GetShaderCache();

        // Read shaders from disk
        JobHandle shaderJobsHandle = k_InvalidJobHandle;
        for (int i = 0; i < 2; ++i)
        {
            auto& shaderPath = k_DefaultShaderPaths[i];
            FixedString64 name = k_ShaderNames[i];
            // Get full path to shader
            IO::FileStream stream(IO::VirtualFileSystem::Get()->GetFullPath(shaderPath));
            std::vector<uint32_t> shaderBytes = stream.ReadFile<uint32_t>();

            // Create shader module
            JobHandle cacheJob = shaderCache.ScheduleCreateShaderJob(
                name,
                m_Renderer.GetGfxDevice(),
                shaderBytes);
            if (cacheJob != k_InvalidJobHandle)
            {
                if (shaderJobsHandle == k_InvalidJobHandle)
                {
                    shaderJobsHandle = cacheJob;
                }
                else
                {
                    JobHandle jobHandles[] = { shaderJobsHandle, cacheJob };
                    shaderJobsHandle = Concurrency::JobManager::Get()->CombineDependencies(jobHandles, 2);
                }
            }
        }
        JobManager::Get()->AwaitCompletion(shaderJobsHandle);
    }
}
