/**
 * @file VkPhysicalDeviceScoringUtils.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-10-08
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "SavannaVk2.h"

namespace savanna::gfx::vk::utils
{
    uint32 ScoreDeviceType(const VkPhysicalDeviceType& deviceType);
    uint32 ScoreDeviceFeatures(const VkPhysicalDeviceFeatures& deviceFeatures);
    uint32 ScoreDeviceLimits(const VkPhysicalDeviceLimits& deviceLimits);

    using ScoreDeviceTypeFunc = decltype(ScoreDeviceType)*;
    using ScoreDeviceFeaturesFunc = decltype(ScoreDeviceFeatures)*;
    using ScoreDeviceLimitsFunc = decltype(ScoreDeviceLimits)*;

    struct VulkanDeviceScoringFuncs
    {
        ScoreDeviceTypeFunc m_DeviceTypeScoringFunc;
        ScoreDeviceFeaturesFunc m_DeviceFeatureScoringFunc;
        ScoreDeviceLimitsFunc m_DeviceLimitsScoringFunc;
    };

    uint32 ScoreDevice(
        VkPhysicalDevice physicalDevice,
        const VulkanDeviceScoringFuncs& scoringFunctionPtrs = VulkanDeviceScoringFuncs{
            ScoreDeviceType,
            ScoreDeviceFeatures,
            ScoreDeviceLimits
        });
} // namespace savanna::Gfx::vk::Utils
