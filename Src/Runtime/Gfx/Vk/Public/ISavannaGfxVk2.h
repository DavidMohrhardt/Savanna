/**
 * @file ISavannaGfxVk2.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief Defines the Vulkan specific interface structures for
 * the Savanna graphics system.
 * @version 0.1
 * @date 2023-09-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef I_SAVANNA_GFX_VK2_H
#define I_SAVANNA_GFX_VK2_H

#include <Public/ISavannaEngine.h>

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
    se_VkQueueFlags_t m_SupportedQueueTypes;
    se_uint32 m_MaxQueueCount;
} se_VkGraphicsCapabilities_t;

/**
 * @brief TODO @david.mohrhardt
 *
 */
typedef struct se_VkInstanceCreateArgs_t
{
    const char** m_ppEnabledLayers;
    se_uint32 m_EnabledLayerCount;

    const char** m_ppEnabledInstanceExtensions;
    se_uint32 m_EnabledInstanceExtensionCount;
} se_VkInstanceCreateArgs_t;

/**
 * @brief Defines creation parameters for the Vulkan physical device.
 */
typedef struct se_VkPhysicalDeviceCreateArgs_t
{
    /**
     * @brief The index of the preferred graphics device. This is can be
     * used to select a specific device if multiple devices are available
     * or reuse a device if the application is restarted.
     */
    se_uint8 m_PreferredGraphicsDeviceIndex;

    /**
     * @brief The names of the device extensions that should be enabled.
     */
    const char** m_ppEnabledDeviceExtensions;

    /**
     * @brief The number of device extensions in the m_ppEnabledDeviceExtensions array.
     */
    se_uint32 m_EnabledDeviceExtensionCount;
} se_VkPhysicalDeviceCreateArgs_t;

/**
 * @brief TODO @david.mohrhardt
 *
 */
typedef struct se_VkLogicalDeviceCreateArgs_t
{
    se_VkQueueFlags_t m_RequestedQueueTypes;
    se_uint32 m_RequestedQueueCount;
    float* m_pQueuePriorities;
} se_VkLogicalDeviceCreateArgs_t;

/**
 * @brief Provides the creation parameters for the Vulkan driver.
 *
 */
typedef struct se_VkDriverCreateInfo_t
{
    se_VkInstanceCreateArgs_t m_InstanceCreateArgs;
    se_VkPhysicalDeviceCreateArgs_t m_PhysicalDeviceCreateArgs;
    se_VkLogicalDeviceCreateArgs_t m_LogicalDeviceCreateArgs;

    void* m_pNext;
    void* m_pUserData;
} se_VkDriverCreateInfo_t;

#endif // !I_SAVANNA_GFX_VK2_H
