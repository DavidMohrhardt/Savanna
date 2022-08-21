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
#include <Vulkan/NativeVulkanDisplaySurfaceInfo.h>
#include <Vulkan/Utilities/VulkanCallbacks.h>

// Savanna Includes
#include <Profiling/Profiler.h>

#define GLFW_INCLUDE_VULKAN
#include <iostream>
#include <string.h>
#include <glm.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

namespace SavannaVulkan
{
    using namespace Savanna;
    using namespace Savanna::Rendering::Vulkan;

    VulkanApplication::VulkanApplication()
        : m_Window(glfwCreateWindow(1920, 1080, "Savanna Vulkan", nullptr, nullptr))
    {
        SAVANNA_INSERT_SCOPED_PROFILER("VulkanApplication::ctor");

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        VulkanInstance instance("Savanna Vulkan", "No Engine", glfwExtensions, glfwExtensionCount);
        if (!instance.IsValid())
        {
            throw std::runtime_error("Failed to create Vulkan instance!");
        }

        m_Renderer = VulkanRenderer(std::move(instance));

        VulkanSurfaceCreateInfoUnion surfaceCreateInfo{};

#if SAVANNA_WINDOWS
        Windows::FillOutSurfaceCreateInfo(GetModuleHandle(nullptr), glfwGetWin32Window(m_Window.GetWindowPtr()), &surfaceCreateInfo);
#else
        #error "Unsupported platform!"
#endif

        // m_Renderer.CreateRenderSurface(surfaceCreateInfo);
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
