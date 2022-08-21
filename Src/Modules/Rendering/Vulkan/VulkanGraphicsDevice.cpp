/**
 * @file VulkanGraphicsDevice.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-20
 *
 */
#include "VulkanGraphicsDevice.h"

#include <Utilities/SavannaCoding.h>
#include <Profiling/Profiler.h>

#include "VulkanPhysicalDevice.h"
#include "Utilities/VulkanMacros.h"

namespace Savanna::Rendering::Vulkan
{
    VulkanGraphicsDevice::VulkanGraphicsDevice()
        : m_Device(VK_NULL_HANDLE)
    {}

    VulkanGraphicsDevice::VulkanGraphicsDevice(
        const VulkanPhysicalDevice& physicalDevice,
        VkDeviceCreateInfo& createInfo)
    {
        SAVANNA_INSERT_SCOPED_PROFILER("VulkanGraphicsDevice::VulkanGraphicsDevice ctor()");
        VK_CALL_OR_THROW(
            vkCreateDevice(physicalDevice.GetPhysicalDevice(), &createInfo, nullptr, &m_Device),
            "Failed to create logical device!");
    }

    VulkanGraphicsDevice::VulkanGraphicsDevice(VulkanGraphicsDevice&& other)
    {
        m_Device = other.m_Device;
        other.m_Device = VK_NULL_HANDLE;
    }

    VulkanGraphicsDevice::~VulkanGraphicsDevice()
    {
        if (m_Device != VK_NULL_HANDLE)
        {
            vkDestroyDevice(m_Device, nullptr);
        }
    }

    VulkanGraphicsDevice& VulkanGraphicsDevice::operator=(VulkanGraphicsDevice&& other)
    {
        m_Device = other.m_Device;
        other.m_Device = VK_NULL_HANDLE;
        return *this;
    }

    VkDevice VulkanGraphicsDevice::GetVkDevice() const { return m_Device; }

    VkQueue VulkanGraphicsDevice::GetVkQueue(const uint32 index) const
    {
        VkQueue outQueue{};
        vkGetDeviceQueue(m_Device, index, 0, &outQueue);
        return outQueue;
    }

} // namespace Savanna::Rendering::Vulkan
