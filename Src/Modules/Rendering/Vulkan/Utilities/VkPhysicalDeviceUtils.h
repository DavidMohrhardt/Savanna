#pragma once

#include "SavannaVk.h"

// Vulkan Includes
#include <vulkan/vulkan.h>

// Savanna Includes
#include <Types/Primitive/PrimitiveTypes.h>

// Module includes
#include "VkPhysicalDeviceDescriptor.h"
#include "VkQueueFamilyIndices.h"

namespace Savanna::Gfx::Vk::Utils
{
    struct RendererCreateInfo;

    typedef bool(*VulkanDeviceTypeScoringFunc)(const VkPhysicalDeviceType&, uint32& outScore);
    typedef bool(*VulkanDeviceFeatureScoringFunc)(const VkPhysicalDeviceFeatures&, uint32& outScore);
    typedef bool(*VulkanDeviceLimitsScoringFunc)(const VkPhysicalDeviceLimits&, uint32& outScore);

    struct VulkanDeviceScoringFuncs
    {
        VulkanDeviceTypeScoringFunc m_DeviceTypeScoringFunc;
        VulkanDeviceFeatureScoringFunc m_DeviceFeatureScoringFunc;
        VulkanDeviceLimitsScoringFunc m_DeviceLimitsScoringFunc;
    };

    static constexpr VulkanDeviceScoringFuncs k_DefaultDeviceChooserScoringFuncs = {
        nullptr, nullptr, nullptr
    };

    void EnumeratePhysicalDeviceDescriptors(
        const VkInstance& instance,
        PhysicalDeviceDescriptor* physicalDeviceDescriptorsPtr,
        uint32& count);

    uint32 GetPhysicalDeviceCount(const VkInstance& instance);

    bool TryChooseVulkanDevice(
        const PhysicalDeviceDescriptor* pDeviceDescs,
        const uint32& deviceDescsCount,
        VkPhysicalDevice* pOutSelectedDevice,
        uint32* pOutSelectedDeviceIndex,
        const VulkanDeviceScoringFuncs& scoringFunctionPtrs = k_DefaultDeviceChooserScoringFuncs);
} // namespace Savanna::Gfx::Vk
