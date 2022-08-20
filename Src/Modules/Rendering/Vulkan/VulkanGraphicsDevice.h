/**
 * @file VulkanGraphicsDevice.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-19
 *
 */
#pragma once

#include <Utilities/SavannaCoding.h>

#include <vulkan/vulkan.h>

namespace Savanna::Rendering::Vulkan
{
    class VulkanPhysicalDevice;

    class VulkanGraphicsDevice
    {
    private:
        VkDevice m_Device;
        VulkanPhysicalDevice* m_PhysicalDevice;

    public:
        VulkanGraphicsDevice() = default;
        VulkanGraphicsDevice(const VulkanGraphicsDevice&) = default;
        VulkanGraphicsDevice(VulkanGraphicsDevice&&) = default;

        VulkanGraphicsDevice(VkDeviceCreateInfo& createInfo);

        ~VulkanGraphicsDevice();
    public:
        VulkanGraphicsDevice& operator=(const VulkanGraphicsDevice&) = default;
        VulkanGraphicsDevice& operator=(VulkanGraphicsDevice&&) = default;
    };
} // namespace Savanna::Rendering::Vulkan
