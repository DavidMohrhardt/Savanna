/**
 * @file ISavannaGfxVk2.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-09-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef I_SAVANNA_GFX_VK2_H
#define I_SAVANNA_GFX_VK2_H

// TODO @DavidMohrhardt: This is a public header, find a better include mechanism.
#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>

typedef enum se_VkQueueFlags_t : uint32_t
{
    se_VkQueueFlags_Graphics = 0x00000001,
    se_VkQueueFlags_Compute = 0x00000002,
    se_VkQueueFlags_Transfer = 0x00000004,
    se_VkQueueFlags_SparseBinding = 0x00000008,
    se_VkQueueFlags_Protected = 0x00000010,
    se_VkQueueFlags_Present = 0x00000020,
    se_VkQueueFlags_All = 0x7FFFFFFF,
    se_VkQueueFlags_None = 0x00000000,
} se_VkQueueFlags_t;

/**
 * @brief Defines the current Vulkan specific capabilities of the graphics system.
 *
 */
typedef struct se_VkGraphicsCapabilities_t
{
    bool m_IsSupported;

    se_VkQueueFlags_t m_SupportedQueueFlags;
    uint32_t m_MaxQueueCount;
} se_VkGraphicsCapabilities_t;

#endif // !I_SAVANNA_GFX_VK2_H
