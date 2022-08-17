/**
 * @file VulkanApplication.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 */

#include "VulkanApplication.h"

#include <Vulkan/Utilities/VulkanCallbacks.h>

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
        : IApplication()
    {
        SAVANNA_INSERT_SCOPED_PROFILER("VulkanApplication::ctor");

        Context& context = GetContext();

        if (!context.TryCreateMemoryArena("Rendering", sizeof(MemoryBlock32KiB))
            || !context.TryCreateMemoryArena("Common", sizeof(MemoryBlock32KiB)))
        {
            // SAVANNA_ERROR_LOG("Failed to create memory arenas");
            throw Savanna::RuntimeErrorException("Failed to create application memory arenas!");
        }

        MemoryArena& defaultArena = context.GetDefaultMemoryArena();
        MemoryArena& commonMemoryArena = context.GetMemoryArena("Common");
        MemoryArena& renderingMemoryArena = context.GetMemoryArena("Rendering");

        FreeListAllocator coreAllocator(&defaultArena, sizeof(MemoryBlock32KiB));
        m_Window = coreAllocator.Allocate<GLFWWindowWrapper>();
        *m_Window = GLFWWindowWrapper(glfwCreateWindow(1920, 1080, "Savanna Vulkan", nullptr, nullptr));

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        for (int i = 0; i < glfwExtensionCount; ++i)
        {
            if (!m_Instance->TryRequestExtension(glfwExtensions[i]))
            {
                throw Savanna::RuntimeErrorException("Required GLFW extension is unsupported by this Vulkan Instance!");
            }
        }

        // m_Instance = coreAllocator.Allocate<VulkanInstance>();
        // *m_Instance(renderingMemoryArena);
        if (!m_Instance->TryCreateInstance("SavannaVulkanRenderer", "No Engine"))
        {
            throw Savanna::RuntimeErrorException("Unable to create Vulkan Instance.");
        }

        m_PhysicalDevice = new VulkanPhysicalDevice(m_Instance->GetInstance());
        m_Initialized = true;
    }

    VulkanApplication::~VulkanApplication()
    {
        SAVANNA_INSERT_SCOPED_PROFILER("VulkanApplication::dtor");

        if (m_Window != nullptr)
        {
            delete m_Window;
        }

        if (m_PhysicalDevice != nullptr)
        {
            delete m_PhysicalDevice;
        }

        if (m_Instance != nullptr)
        {
            delete m_Instance;
        }

        m_Initialized = false;
    }

    void VulkanApplication::Run()
    {
        if (m_Initialized)
        {
            while(!m_Window->ShouldClose())
            {
                m_Window->PollEvents();
            }
        }
    }
}
