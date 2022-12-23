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

    public:
        VulkanGraphicsDevice();

        VulkanGraphicsDevice(const VulkanGraphicsDevice&) = delete;
        VulkanGraphicsDevice(VulkanGraphicsDevice&&);

        VulkanGraphicsDevice(const VulkanPhysicalDevice& physicalDevice, const VkDeviceCreateInfo* inCreateInfo);

        ~VulkanGraphicsDevice();

    public:
        VulkanGraphicsDevice& operator=(VulkanGraphicsDevice&) = delete;
        VulkanGraphicsDevice& operator=(VulkanGraphicsDevice&&);

        operator VkDevice() const { return m_Device; }
        operator VkDevice*() { return &m_Device; }
        operator const VkDevice*() const { return &m_Device; }
        operator VkDevice&() { return m_Device; }
        operator const VkDevice&() const { return m_Device; }

    public:
        VkDevice GetVkDevice() const;
        VkQueue GetVkQueue(const uint32 index) const;
    };
} // namespace Savanna::Rendering::Vulkan
