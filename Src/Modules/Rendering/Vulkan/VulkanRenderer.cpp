/**
 * @file VulkanRenderer.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-20
 *
 */

#include "VulkanRenderer.h"

#include <Profiling/Profiler.h>

#include "Utilities/VulkanDeviceChooser.h"

#include <vector>

namespace Savanna::Rendering::Vulkan
{
    VulkanRenderer::VulkanRenderer()
        : m_Instance("Savanna Vulkan Renderer", "No Engine")
    {
        SAVANNA_INSERT_SCOPED_PROFILER("VulkanRenderer::VulkanRenderer ctor()");
        if (!m_Instance.IsValid())
        {
            throw std::runtime_error("Failed to create Vulkan instance!");
        }

        VulkanPhysicalDeviceDescriptor selectedDeviceDescriptor{};

        uint32 physicalDeviceCount = VulkanPhysicalDevice::GetPhysicalDeviceCount(m_Instance.GetVkInstance());

        std::vector<VulkanPhysicalDeviceDescriptor> deviceDescriptors =
            std::vector<VulkanPhysicalDeviceDescriptor>(physicalDeviceCount);

        VulkanPhysicalDevice::GetPhysicalDeviceDescriptors(
            m_Instance.GetVkInstance(), physicalDeviceCount, deviceDescriptors.data());

        if (!TryChooseVulkanDeviceDescriptor(
            deviceDescriptors.data(), physicalDeviceCount, selectedDeviceDescriptor))
        {
            throw std::runtime_error("No suitable Vulkan devices found.");
        }

        m_PhysicalDevice = VulkanPhysicalDevice(selectedDeviceDescriptor);

        VulkanQueueFamilyIndices indices = m_PhysicalDevice.GetQueueFamilyIndices();
        if (!indices.m_GraphicsQueueFamilyIndex.has_value())
        {
            throw std::runtime_error("No suitable Vulkan queues found.");
        }

        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = indices.m_GraphicsQueueFamilyIndex.value();
        queueCreateInfo.queueCount = 1;

        const float queuePriority = 1.0f;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        VkPhysicalDeviceFeatures deviceFeatures{};
        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = 1;
        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = 0;

        m_GraphicsDevice = VulkanGraphicsDevice(m_PhysicalDevice, createInfo);
        m_GraphicsQueue = m_GraphicsDevice.GetVkQueue(indices.m_GraphicsQueueFamilyIndex.value());
    }
} // namespace Savanna::Rendering::Vulkan
