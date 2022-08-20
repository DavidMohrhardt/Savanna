/**
 * @file VulkanPhysicalDevice.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 */

#include "VulkanPhysicalDevice.h"

#include <cstdlib>
#include <mutex>
#include <cassert>
#include <vector>
#include <stdexcept>

#include <Profiling/Profiler.h>
#include <Types/Exceptions/SavannaException.h>
#include <Types/Strings/FixedString.h>

namespace Savanna::Rendering::Vulkan
{
    uint32 VulkanPhysicalDevice::GetPhysicalDeviceCount(const VkInstance& instance)
    {
        SAVANNA_INSERT_SCOPED_PROFILER("VulkanPhysicalDevice::GetPhysicalDeviceCount");
        static uint32 s_count = 0;
        if (s_count == 0) SAVANNA_BRANCH_UNLIKELY
        {
            uint32 count = 0;
            vkEnumeratePhysicalDevices(instance, &count, nullptr);
            s_count = count;
        }
        return s_count;
    }

    void VulkanPhysicalDevice::GetPhysicalDeviceDescriptors(
        const VkInstance& instance,
        uint32 count,
        VulkanPhysicalDeviceDescriptor* physicalDeviceDescriptorsPtr)
    {
        SAVANNA_INSERT_SCOPED_PROFILER("VulkanPhysicalDevice::GetPhysicalDeviceDescriptors");
        if (physicalDeviceDescriptorsPtr != nullptr)
        {
            std::vector<VkPhysicalDevice> physicalDevices(count);
            vkEnumeratePhysicalDevices(instance, &count, physicalDevices.data());
            for (int i = 0; i < count; ++i)
            {
                physicalDeviceDescriptorsPtr[i].physicalDevice = physicalDevices[i];
                vkGetPhysicalDeviceProperties(physicalDevices[i], &physicalDeviceDescriptorsPtr[i].properties);
                vkGetPhysicalDeviceFeatures(physicalDevices[i], &physicalDeviceDescriptorsPtr[i].features);
                vkGetPhysicalDeviceMemoryProperties(physicalDevices[i], &physicalDeviceDescriptorsPtr[i].memoryProperties);
            }
        }
    }

    VulkanPhysicalDevice::VulkanPhysicalDevice() : m_Descriptor({ VK_NULL_HANDLE, {0}, {0}, {0} }) {}

    VulkanPhysicalDevice::VulkanPhysicalDevice(const VulkanPhysicalDeviceDescriptor& descriptor)
    {
        SAVANNA_INSERT_SCOPED_PROFILER("VulkanPhysicalDevice::VulkanPhysicalDevice ctor(const VulkanPhysicalDeviceDescriptor&)");
        m_Descriptor = descriptor;
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(GetPhysicalDevice(), &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(GetPhysicalDevice(), &queueFamilyCount, queueFamilyProperties.data());
        uint32 i = 0;
        for (const auto& properties : queueFamilyProperties)
        {
            VkQueueFlags flags = properties.queueFlags;

            if (flags & VK_QUEUE_GRAPHICS_BIT)
            {
                m_QueueFamilyIndices.m_GraphicsQueueFamilyIndex = static_cast<uint32>(i);
            }

            if (flags & VK_QUEUE_COMPUTE_BIT)
            {
                m_QueueFamilyIndices.m_ComputeQueueFamilyIndex = static_cast<uint32>(i);
            }

            if (flags & VK_QUEUE_TRANSFER_BIT)
            {
                m_QueueFamilyIndices.m_TransferQueueFamilyIndex = static_cast<uint32>(i);
            }

            if (flags & VK_QUEUE_SPARSE_BINDING_BIT)
            {
                m_QueueFamilyIndices.m_SparseBindingQueueFamilyIndex = static_cast<uint32>(i);
            }

            if (m_QueueFamilyIndices.HasAllQueueFamilyIndices())
            {
                break;
            }

            ++i;
        }
    }

    const VulkanPhysicalDeviceDescriptor& VulkanPhysicalDevice::GetDescriptor() const
    {
        return m_Descriptor;
    }

    const VkPhysicalDevice& VulkanPhysicalDevice::GetPhysicalDevice() const
    {
        return m_Descriptor.physicalDevice;
    }

    const VkPhysicalDeviceProperties& VulkanPhysicalDevice::GetProperties() const
    {
        return m_Descriptor.properties;
    }

    const VkPhysicalDeviceFeatures& VulkanPhysicalDevice::GetFeatures() const
    {
        return m_Descriptor.features;
    }

    const VkPhysicalDeviceMemoryProperties& VulkanPhysicalDevice::GetMemoryProperties() const
    {
        return m_Descriptor.memoryProperties;
    }

    const VkPhysicalDeviceLimits& VulkanPhysicalDevice::GetLimits() const
    {
        return m_Descriptor.properties.limits;
    }

    const VkPhysicalDeviceSparseProperties& VulkanPhysicalDevice::GetSparseProperties() const
    {
        return m_Descriptor.properties.sparseProperties;
    }

    const VulkanQueueFamilyIndices& VulkanPhysicalDevice::GetQueueFamilyIndices() const
    {
        return m_QueueFamilyIndices;
    }
} // namespace Savanna::Rendering::Vulkan
