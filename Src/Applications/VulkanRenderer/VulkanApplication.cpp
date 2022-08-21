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
        for (int i = 0; i < glfwExtensionCount; ++i)
        {
            if (!m_Renderer.GetVulkanInstance().TryRequestExtension(glfwExtensions[i]))
            {
                throw Savanna::RuntimeErrorException("Required GLFW extension is unsupported by this Vulkan Instance!");
            }
        }
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
