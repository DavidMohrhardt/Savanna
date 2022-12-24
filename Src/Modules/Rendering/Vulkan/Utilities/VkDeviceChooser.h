/**
 * @file VkDeviceChooser.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-19
 *
 */
#pragma once

#include <vulkan/vulkan.h>

#include <VkPhysicalDevice.h>
#include <VkRendererCreateInfo.h>

namespace Savanna::Rendering::Vk
{
    typedef bool(*VkDeviceTypeScoringFunc)(const VkPhysicalDeviceType&, uint32& outScore);
    typedef bool(*VkDeviceFeatureScoringFunc)(const VkPhysicalDeviceFeatures&, uint32& outScore);
    typedef bool(*VkDeviceLimitsScoringFunc)(const VkPhysicalDeviceLimits&, uint32& outScore);

    struct VkDeviceScoringFuncs
    {
        VkDeviceTypeScoringFunc m_DeviceTypeScoringFunc;
        VkDeviceFeatureScoringFunc m_DeviceFeatureScoringFunc;
        VkDeviceLimitsScoringFunc m_DeviceLimitsScoringFunc;
    };

    inline static const VkDeviceScoringFuncs k_DefaultDeviceChooserScoringFuncs = {
        nullptr, nullptr, nullptr
    };

    bool TryChooseVkDevice(
        const VkPhysicalDevice* devices,
        const uint32& count,
        const VkRendererCreateInfo* pCreateInfo,
        VkPhysicalDevice* pOutSelectedDevice,
        const VkDeviceScoringFuncs& scoringFunctionPtrs = k_DefaultDeviceChooserScoringFuncs);
} // namespace Savanna::Vk
