/**
 * @file VkRendererCreateInfo.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-21
 *
 */
#pragma once

#include <Utilities/SavannaCoding.h>
#include <Types/Strings/FixedString.h>

#include "VkQueueFlags.h"
#include "VkSurfaceCreateInfos.h"

#include <vulkan/vulkan.h>

// TODO @DavidMohrhardt - This is far too cumbersome. Implement a better structure creating both VK and DX12 renderers
typedef struct se_VkRendererCreateInfo_t
{
    const char* m_ApplicationName;
    const char* m_EngineName;

    se_uint32 m_Width;
    se_uint32 m_Height;

    se_Version m_ApplicationVersion;
    se_Version m_EngineVersion;

    const char** m_ppEnabledLayerNames;
    se_uint32 m_EnabledLayerCount;

    const char** m_ppInstanceExtensions;
    se_uint32 m_InstanceExtensionsCount;

    const char** m_ppDeviceExtensions;
    se_uint32 m_DeviceExtensionsCount;

    se_VkSurfaceCreateInfo_t m_SurfaceCreateInfo;
    se_uint32 m_QueueFlags;

    VkAllocationCallbacks* m_pAllocationCallbacks;
    const void* m_pUserData;
} se_VkRendererCreateInfo_t;
DECLARE_SAVANNA_EXTENDED_NAMESPACED_CPP_TYPE_DEF(Gfx::Vk, se_VkRendererCreateInfo_t, RendererCreateInfo);
