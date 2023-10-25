/**
 * @file VkGpu.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-10-08
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "SavannaVk2.h"
#include "VkQueueFamilyIndices.h"

namespace Savanna::Gfx::Vk2
{
    class VkGpu
    {
    private:
        friend class VkDriver;

        VkPhysicalDevice m_PhysicalDevice;
        VkDevice m_LogicalDevice;
        VkPhysicalDeviceProperties m_PhysicalDeviceProperties;
        VkQueueFamilyIndices m_QueueFamilyIndices;


        VkGpu() = default;
        ~VkGpu() = default;

        VkGpu(const VkGpu&) = delete;
        VkGpu(VkGpu&&) = delete;

        bool InitializeLogicalDevice(
            const se_VkDriverCreateInfo_t &createInfo,
            VkInstance &instance,
            VkSurfaceKHR& surface,
            const VkAllocationCallbacks* pAllocationCallbacks);

        bool TryInitialize(const se_VkDriverCreateInfo_t &createInfo,
                            VkInstance &instance,
                            VkSurfaceKHR& surface,
                            const VkAllocationCallbacks* pAllocationCallbacks);
        void Reset(
            VkInstance& instance,
            const VkAllocationCallbacks* pAllocationCallbacks);

    public:
        inline operator VkPhysicalDevice*() { return &m_PhysicalDevice; }
        inline operator const VkPhysicalDevice*() const { return &m_PhysicalDevice; }
        inline operator VkPhysicalDevice&() { return m_PhysicalDevice; }
        inline operator const VkPhysicalDevice&() const { return m_PhysicalDevice; }

        inline operator VkDevice*() { return &m_LogicalDevice; }
        inline operator const VkDevice*() const { return &m_LogicalDevice; }
        inline operator VkDevice&() { return m_LogicalDevice; }
        inline operator const VkDevice&() const { return m_LogicalDevice; }

        const VkQueueFamilyIndices& GetQueueFamilyIndices() const { return m_QueueFamilyIndices; }
    };
} // namespace Savanna::Gfx::Vk2
