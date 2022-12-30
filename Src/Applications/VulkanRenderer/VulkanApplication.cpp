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
        using namespace Savanna;
        using namespace Savanna::Gfx::Vk;
        SAVANNA_INSERT_SCOPED_PROFILER("VulkanApplication::ctor");

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        const uint32_t deviceExtensionsCount = 1;

        RendererCreateInfo rendererCreateInfo{};
        rendererCreateInfo.m_ApplicationName = &k_ApplicationName;
        rendererCreateInfo.m_EngineName = &k_EngineName;
        rendererCreateInfo.m_InstanceExtensions = glfwExtensions;
        rendererCreateInfo.m_InstanceExtensionsCount = glfwExtensionCount;

#if SAVANNA_WINDOWS
        Windows::FillOutSurfaceCreateInfo(GetModuleHandle(nullptr), glfwGetWin32Window(m_Window.GetWindowPtr()), &rendererCreateInfo.m_SurfaceCreateInfo);
#else
        #error "Unsupported platform!"
#endif

        // TODO @DavidMohrhardt Allow for additional extensions to be added via queries
        auto swapChainExtensionName = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
        rendererCreateInfo.m_DeviceExtensions = &swapChainExtensionName;
        rendererCreateInfo.m_DeviceExtensionsCount = 1;

        m_Renderer = Renderer(&rendererCreateInfo);
    }

    VulkanApplication::~VulkanApplication()
    {
        SAVANNA_INSERT_SCOPED_PROFILER("VulkanApplication::dtor");
    }

    void VulkanApplication::Run()
    {
        while(!m_Window.ShouldClose())
        {
            m_Window.PollEvents();
        }
    }
}
