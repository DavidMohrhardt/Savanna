/**
 * @file VulkanRendererCreateInfo.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-21
 *
 */
#pragma once

#include <Utilities/SavannaCoding.h>
#include <Types/Strings/FixedString.h>

#include "NativeVulkanDisplaySurfaceInfo.h"

#include <vulkan/vulkan.h>

namespace Savanna::Rendering::Vulkan
{
    struct VulkanRendererCreateInfo
    {
        FixedString32 m_ApplicationName;
        FixedString32 m_EngineName;

        const char** m_ActiveExtensions;
        uint32 m_ActiveExtensionCount;

        VulkanSurfaceCreateInfoUnion m_SurfaceCreateInfo;
    };
} // namespace Savanna::Rendering::Vulkan
