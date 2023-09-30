/**
 * @file SavannaVk2.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
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

DECLARE_SAVANNA_EXTENDED_NAMESPACED_CPP_TYPE_DEF(Gfx::Vk2, se_VkGraphicsCapabilities_t, VkGraphicsCapabilities);

namespace Savanna::Gfx::Vk2
{
    se_GfxErrorCode_t AcquireDriver(
        const se_GfxDriverCreateInfo_t* const pCreateInfo,
        IGfxDriver** ppDriver,
        void* pUserData,
        const se_AllocatorInterface_t allocatorInterface = Savanna::MemoryManager::GetAllocatorInterfaceForLabel(k_SavannaMemoryLabelGfx));
}

#endif // !SAVANNA_GFX_VK2_H
