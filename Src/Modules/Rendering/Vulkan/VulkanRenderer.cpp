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
    VulkanRenderer::VulkanRenderer(const FixedString32& applicationName, const FixedString32& engineName)
    {
        SAVANNA_INSERT_SCOPED_PROFILER("VulkanRenderer::VulkanRenderer ctor()");
        m_Instance = VulkanInstance(applicationName, engineName, nullptr, 0);

        if (!m_Instance.IsValid())
        {
            throw RuntimeErrorException("Failed to create Vulkan instance!");
        }

        SelectPhysicalDevice(m_Instance, &m_PhysicalDevice);
        CreateLogicalDevice(m_PhysicalDevice, &m_GraphicsDevice);
        QueryGraphicsQueue(m_PhysicalDevice, m_GraphicsDevice, &m_GraphicsQueue);
    }

    VulkanRenderer::VulkanRenderer(VulkanInstance&& instance)
    {
        SAVANNA_INSERT_SCOPED_PROFILER("VulkanRenderer::VulkanRenderer ctor(instance)");
        if (!instance.IsValid())
        {
            throw RuntimeErrorException("Failed to create Vulkan instance!");
        }

        m_Instance = std::move(instance);
        SelectPhysicalDevice(m_Instance, &m_PhysicalDevice);
        CreateLogicalDevice(m_PhysicalDevice, &m_GraphicsDevice);
        QueryGraphicsQueue(m_PhysicalDevice, m_GraphicsDevice, &m_GraphicsQueue);
    }

    VulkanRenderer::VulkanRenderer(VulkanInstance &&instance, VulkanPhysicalDevice &&physicalDevice, VulkanGraphicsDevice &&graphicsDevice)
        : m_Instance(std::move(instance))
        , m_PhysicalDevice(std::move(physicalDevice))
        , m_GraphicsDevice(std::move(graphicsDevice))
    {
        SAVANNA_INSERT_SCOPED_PROFILER("VulkanRenderer::VulkanRenderer ctor(VulkanInstance&, VulkanPhysicalDevice&, VulkanGraphicsDevice&)");
        if (!m_Instance.IsValid())
        {
            throw RuntimeErrorException("Failed to create Vulkan instance!");
        }
    }

    void VulkanRenderer::SelectPhysicalDevice(
        const VulkanInstance& instance,
        VulkanPhysicalDevice* outPhysicalDevice)
    {
        SAVANNA_INSERT_SCOPED_PROFILER("VulkanRenderer::SelectPhysicalDevice()");
        assert(outPhysicalDevice != nullptr && "outPhysicalDevice is nullptr!");

        uint32 physicalDeviceCount = VulkanPhysicalDevice::GetPhysicalDeviceCount(instance.GetVkInstance());

        std::vector<VulkanPhysicalDeviceDescriptor> deviceDescriptors =
            std::vector<VulkanPhysicalDeviceDescriptor>(physicalDeviceCount);

        VulkanPhysicalDevice::GetPhysicalDeviceDescriptors(
            instance.GetVkInstance(), physicalDeviceCount, deviceDescriptors.data());

        VulkanPhysicalDeviceDescriptor selectedDeviceDescriptor{};

        if (!TryChooseVulkanDeviceDescriptor(
            deviceDescriptors.data(), physicalDeviceCount, selectedDeviceDescriptor))
        {
            throw RuntimeErrorException("No suitable Vulkan devices found.");
        }

        *outPhysicalDevice = VulkanPhysicalDevice(selectedDeviceDescriptor);
    }

    void VulkanRenderer::CreateLogicalDevice(const VulkanPhysicalDevice& physicalDevice, VulkanGraphicsDevice* outGfxDevice)
    {
        SAVANNA_INSERT_SCOPED_PROFILER("VulkanRenderer::CreateLogicalDevice()");
        assert(outGfxDevice != nullptr && "outGfxDevice is nullptr!");

        VulkanQueueFamilyIndices indices = physicalDevice.GetQueueFamilyIndices();
        if (!indices.m_GraphicsQueueFamilyIndex.has_value())
        {
            throw RuntimeErrorException("No suitable graphics queues found.");
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

        *outGfxDevice = VulkanGraphicsDevice(physicalDevice, createInfo);
    }

    void VulkanRenderer::QueryGraphicsQueue(
        const VulkanPhysicalDevice& physicalDevice,
        const VulkanGraphicsDevice& device,
        VkQueue* outGraphicsQueue)
    {
        SAVANNA_INSERT_SCOPED_PROFILER("VulkanRenderer::QueryGraphicsQueue()");
        VulkanQueueFamilyIndices indices = physicalDevice.GetQueueFamilyIndices();
        if (!indices.m_GraphicsQueueFamilyIndex.has_value())
        {
            throw RuntimeErrorException("No suitable graphics queues found.");
        }

        *outGraphicsQueue = device.GetVkQueue(indices.m_GraphicsQueueFamilyIndex.value());
    }
} // namespace Savanna::Rendering::Vulkan
