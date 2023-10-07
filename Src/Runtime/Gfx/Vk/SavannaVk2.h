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
#include "VkDriver.h"

namespace Savanna::Gfx::Vk2
{
    // Using Declarations for ISavannaGfxVk2.h types
    using VkGraphicsCapabilities = se_VkGraphicsCapabilities_t;

    using VkQueueKind = Enumeration<se_VkQueueKind_t, uint8_t>;
    using VkQueueFlags = FlagEnumeration<se_VkQueueFlags_t, uint8_t>;

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
