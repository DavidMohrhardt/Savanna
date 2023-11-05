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
#include <vulkan/vulkan.h>

// Savanna Vulkan Includes
#include "Public/ISavannaGfx.h"
#include "Public/ISavannaGfxVk2.h"

#define VK_MUST_SUCCEED(__input, __message) \
    { \
        VkResult __vk_result = __input; \
        if (__vk_result != VK_SUCCESS) \
        { \
            throw Savanna::RuntimeErrorException("[Vulkan] " __message); \
        } \
    }

#define VK_MUST_SUCCEED_RETURN(__input, __message, __returnValue) \
    { \
        auto __vk_result = __input; \
        if (__vk_result != VK_SUCCESS) \
        { \
            SAVANNA_FATAL_LOG("[Vulkan] {}", __message); \
            return __returnValue; \
        } \
    }

namespace Savanna::Gfx::Vk2
{
    // Using Declarations for ISavannaGfxVk2.h types
    using VkGraphicsCapabilities = se_VkGraphicsCapabilities_t;

    using VkQueueKind = Enumeration<se_VkQueueKind_t, uint8_t>;
    using VkQueueFlags = FlagEnumeration<se_VkQueueFlags_t, uint8_t>;

    inline static VkAllocationCallbacks* g_VkAllocationCallbacks = nullptr;

    /**
     * @brief
     *
     * @param pCreateInfo
     * @param ppDriver
     * @param pUserData
     * @param allocatorInterface
     * @return se_GfxErrorCode_t
     */
    se_GfxErrorCode_t GetDriverInterface(se_GfxDriverInterface_t& outDriverInterface);
}

#endif // !SAVANNA_GFX_VK2_H
