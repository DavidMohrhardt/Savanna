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
#include <Vulkan/Utilities/VulkanDeviceChooser.h>

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
        : IApplication()
        , m_Window(glfwCreateWindow(1920, 1080, "Savanna Vulkan", nullptr, nullptr))
        , m_Instance("SavannaVulkanRenderer", "No Engine")
    {
        SAVANNA_INSERT_SCOPED_PROFILER("VulkanApplication::ctor");

        if (m_Instance.GetErrorCode() != VK_SUCCESS)
        {
            throw Savanna::RuntimeErrorException("Unable to create Vulkan Instance.");
        }

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        for (int i = 0; i < glfwExtensionCount; ++i)
        {
            if (!m_Instance.TryRequestExtension(glfwExtensions[i]))
            {
                throw Savanna::RuntimeErrorException("Required GLFW extension is unsupported by this Vulkan Instance!");
            }
        }

        // Get Physical Devices
        {
            SAVANNA_INSERT_SCOPED_PROFILER("VulkanApplication::ctor - Get Physical Devices");
            uint32 count = VulkanPhysicalDevice::GetPhysicalDeviceCount(m_Instance.GetInstance());
            std::vector<VulkanPhysicalDeviceDescriptor> physicalDeviceDescriptors(count);

            VulkanPhysicalDevice::GetPhysicalDeviceDescriptors(
                m_Instance.GetInstance(),
                count,
                physicalDeviceDescriptors.data());

            for (int i = 0; i < count; ++i)
            {
                VulkanPhysicalDevice physicalDevice = VulkanPhysicalDevice(physicalDeviceDescriptors[i]);
                m_AvailablePhysicalDevices.push_back(physicalDevice);
            }

            bool foundSuitableDevice = TryChooseVulkanDevice(m_AvailablePhysicalDevices.data(), m_AvailablePhysicalDevices.size(), m_CurrentPhysicalDevice);

            if (!foundSuitableDevice)
            {
                throw Savanna::RuntimeErrorException("Unable to choose a Vulkan Physical Device!");
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
