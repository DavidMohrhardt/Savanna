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

#include "VkRendererCreateFlags.h"
#include "VkRendererQueueFlags.h"
#include "VkSurfaceCreateInfos.h"

#include <vulkan/vulkan.h>

typedef struct seVkRendererCreateInfo_t
{
    const char** m_ApplicationName;
    const char** m_EngineName;

    seVkRendererCreateFlags_t m_CreateFlags;
    seVkRendererQueueFlags_t m_QueueFlags;

    const char** m_InstanceExtensions;
    __se_uint32 m_InstanceExtensionsCount;

    const char** m_DeviceExtensions;
    __se_uint32 m_DeviceExtensionsCount;

    const char** m_EnabledLayerNames;
    __se_uint32 m_EnabledLayerCount;

    seVkSurfaceCreateInfo_t m_SurfaceCreateInfo;
    VkSwapchainCreateInfoKHR m_SwapchainCreateInfo;

    VkAllocationCallbacks* m_pAllocationCallbacks;
    const void* m_pUserData;
} seVkRendererCreateInfo_t;
DECLARE_SAVANNA_EXTENDED_NAMESPACED_CPP_TYPE_DEF(Gfx::Vk, seVkRendererCreateInfo_t, RendererCreateInfo);
