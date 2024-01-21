/**
 * @file SavannaVk2.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief The internal Vulkan Gfx header for Savanna.
 * @version 0.1
 * @date 2023-09-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef SAVANNA_GFX_VK2_H
#define SAVANNA_GFX_VK2_H

// Core Engine Includes
#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>

// Vulkan Includes
#include "VkIncludes.h"

// Savanna Vulkan Includes
#include "Public/ISavannaGfx.h"
#include "Public/ISavannaGfxVk2.h"

#define VK_SUCCEEDED(__vkResult) __vkResult == VK_SUCCESS
#define VK_FAILED(__vkResult) __vkResult != VK_SUCCESS

#define VK_MUST_SUCCEED(__input, __message) \
    { \
        VkResult __vkResult__ = __input; \
        if (VK_FAILED(__vkResult__)) \
        { \
            SAVANNA_FATAL_LOG("[Vulkan] {}", __message); \
            return; \
        } \
    }

#define VK_MUST_SUCCEED_RETURN(__input, __message, __returnValue) \
    { \
        auto __vkResult__ = __input; \
        if (VK_FAILED(__vkResult__)) \
        { \
            SAVANNA_FATAL_LOG("[Vulkan] {}", __message); \
            return __returnValue; \
        } \
    }

namespace savanna::gfx::vk
{
    // Using Declarations for ISavannaGfxVk2.h types
    using VkGraphicsCapabilities = seVkGraphicsCapabilities;
    using VkQueueKind = Enumeration<seVkQueueKind, uint8_t>;
    using VkQueueFlags = FlagEnumeration<seVkQueueFlags, uint8_t>;

    // inline static VkAllocationCallbacks* g_VkAllocationCallbacks = nullptr;

    inline seGfxErrorCode GetErrorCode(VkResult result);
}

/**
 * @brief TODO @David.Mohrhardt Document
 *
 * @param pCreateInfo
 * @param ppDriver
 * @param pUserData
 * @param allocatorInterface
 * @return seGfxErrorCode
 */
seGfxErrorCode GetDriverInterfaceVulkan(seGfxDriverInterface& outDriverInterface);

#endif // !SAVANNA_GFX_VK2_H
