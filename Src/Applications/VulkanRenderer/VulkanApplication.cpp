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
#include <Vulkan/VkSurfaceCreateInfos.h>
#include <Vulkan/Utilities/VkCallbacks.h>
#include <Vulkan/Utilities/VkLayerUtils.h>
#include <Vulkan/Utilities/VkExtensionUtils.h>
#include <Vulkan/Windows/WindowsVkDisplaySurface.h>

// Savanna Includes
#include <Profiling/Profiler.h>

#define GLFW_INCLUDE_VULKAN
#include <iostream>
#include <string.h>
#include <glm.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

namespace Savanna::Application
{
    const char* k_ApplicationName = "Savanna";
    const char* k_EngineName = "No Engine";

    VulkanApplication::VulkanApplication()
        : m_Window(glfwCreateWindow(1920, 1080, "Savanna", nullptr, nullptr))
    {
        SAVANNA_INSERT_SCOPED_PROFILER(VulkanApplication::ctor);
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
            // VK_KHR_SURFACE_EXTENSION_NAME,
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        };
        rendererCreateInfo.m_ppDeviceExtensions = deviceExtensions.data();
        rendererCreateInfo.m_DeviceExtensionsCount = static_cast<uint32>(deviceExtensions.size());

        m_Renderer.Initialize(&rendererCreateInfo);
    }

    VulkanApplication::~VulkanApplication()
    {
        SAVANNA_INSERT_SCOPED_PROFILER(VulkanApplication::~VulkanApplication());
    }

    void VulkanApplication::Run()
    {
        while(!m_Window.ShouldClose())
        {
            m_Window.PollEvents();
        }
    }
}
