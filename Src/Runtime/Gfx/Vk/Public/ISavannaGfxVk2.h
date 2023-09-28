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

typedef enum se_VkQueueKind_t : uint8_t
{
    se_VkQueueKindNone,
    se_VkQueueKindGraphics,
    se_VkQueueKindCompute,
    se_VkQueueKindTransfer,
    se_VkQueueKindSparseBinding,
    se_VkQueueKindPresent,
} se_VkQueueKind_t;

typedef enum se_VkQueueFlags_t : uint8_t
{
    se_VkRequestedQueueFlagsNone = 0x0,
    se_VkRequestedQueueFlagsAll = 0xFF,

    se_VkRequestedQueueFlagsGraphics = 0x1 << se_VkQueueKindGraphics,
    se_VkRequestedQueueFlagsCompute = 0x1 << se_VkQueueKindCompute,
    se_VkRequestedQueueFlagsTransfer = 0x1 << se_VkQueueKindTransfer,
    se_VkRequestedQueueFlagsSparseBinding = 0x1 << se_VkQueueKindSparseBinding,
    se_VkRequestedQueueFlagsPresent = 0x1 << se_VkQueueKindPresent,
} se_VkRequestedQueueFlags_t;

/**
 * @brief Defines the current Vulkan specific capabilities of the graphics system.
 *
 */
typedef struct se_VkGraphicsCapabilities_t
{
    bool m_IsSupported;
} se_VkGraphicsCapabilities_t;

/**
 * @brief Provides the creation parameters for the Vulkan driver.
 *
 */
typedef struct se_VkDriverCreateInfo_t
{
    const char** m_ppEnabledInstanceExtensions;
    uint32_t m_EnabledInstanceExtensionCount;

    const char** m_ppEnabledDeviceExtensions;
    uint32_t m_EnabledDeviceExtensionCount;

    const char** m_ppEnabledLayers;
    uint32_t m_EnabledLayerCount;

    const char** m_ppEnabledDeviceLayers;
    uint32_t m_EnabledDeviceLayerCount;
} se_VkDriverCreateInfo_t;

#endif // !I_SAVANNA_GFX_VK2_H
