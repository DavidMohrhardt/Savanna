/**
 * @file VkDeviceChooser.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-19
 *
 */
#pragma once

#include <SavannaVk.h>

#include <vulkan/vulkan.h>

#include <VkRendererCreateInfo.h>

namespace Savanna::Gfx::Vulkan
{
    typedef bool(*VkDeviceTypeScoringFunc)(const VkPhysicalDeviceType&, uint32& outScore);
    typedef bool(*VkDeviceFeatureScoringFunc)(const VkPhysicalDeviceFeatures&, uint32& outScore);
    typedef bool(*VkDeviceLimitsScoringFunc)(const VkPhysicalDeviceLimits&, uint32& outScore);

    struct VkPhysicalDeviceScoringFuncs
    {
        VkDeviceTypeScoringFunc m_DeviceTypeScoringFunc;
        VkDeviceFeatureScoringFunc m_DeviceFeatureScoringFunc;
        VkDeviceLimitsScoringFunc m_DeviceLimitsScoringFunc;
    };

    namespace Utils
    {
        void SortPhysicalDevicesByScore(
            std::vector<VkPhysicalDevice>& devices,
            const VkRendererCreateInfo& createInfo);

        bool TryChoosePhysicalDevice(
            const VkPhysicalDevice* pDevices,
            const uint32& count,
            const VkRendererCreateInfo& createInfo,
            VkPhysicalDevice& outSelectedDevice);
    }
} // namespace Savanna::Vk
