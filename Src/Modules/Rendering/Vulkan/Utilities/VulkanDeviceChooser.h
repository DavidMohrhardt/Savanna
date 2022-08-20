/**
 * @file VulkanDeviceChooser.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-19
 *
 */
#pragma once

#include <vulkan/vulkan.h>

#include "VulkanPhysicalDevice.h"

namespace Savanna::Rendering::Vulkan
{
    typedef bool(*VulkanDeviceTypeScoringFunc)(const VkPhysicalDeviceType&, uint32& outScore);
    typedef bool(*VulkanDeviceFeatureScoringFunc)(const VkPhysicalDeviceFeatures&, uint32& outScore);
    typedef bool(*VulkanDeviceLimitsScoringFunc)(const VkPhysicalDeviceLimits&, uint32& outScore);

    struct VulkanDeviceScoringFuncs
    {
        VulkanDeviceTypeScoringFunc m_DeviceTypeScoringFunc;
        VulkanDeviceFeatureScoringFunc m_DeviceFeatureScoringFunc;
        VulkanDeviceLimitsScoringFunc m_DeviceLimitsScoringFunc;
    };

    inline static const VulkanDeviceScoringFuncs k_DefaultDeviceChooserScoringFuncs = {
        nullptr, nullptr, nullptr
    };

    bool TryChooseVulkanDevice(
        const VulkanPhysicalDevice* devices,
        const size_t& count,
        VulkanPhysicalDevice& selectedDevice,
        const VulkanDeviceScoringFuncs& scoringFunctionPtrs = k_DefaultDeviceChooserScoringFuncs);

    bool TryChooseVulkanDeviceDescriptor(
        const VulkanPhysicalDeviceDescriptor* deviceDescriptors,
        const size_t& count,
        VulkanPhysicalDeviceDescriptor& selectedDeviceDesc,
        const VulkanDeviceScoringFuncs& scoringFunctionPtrs = k_DefaultDeviceChooserScoringFuncs);
} // namespace Savanna::Vulkan
