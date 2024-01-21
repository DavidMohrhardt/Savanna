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

typedef enum seVkQueueKind : uint8_t
{
    se_VkQueueKindNone,
    se_VkQueueKindGraphics,
    se_VkQueueKindCompute,
    se_VkQueueKindTransfer,
    se_VkQueueKindSparseBinding,
    se_VkQueueKindPresent,
} seVkQueueKind;

typedef enum seVkQueueFlags : uint8_t
{
    se_VkRequestedQueueFlagsNone = 0x0,
    se_VkRequestedQueueFlagsAll = 0xFF,

    se_VkRequestedQueueFlagsGraphics = 0x1 << se_VkQueueKindGraphics,
    se_VkRequestedQueueFlagsCompute = 0x1 << se_VkQueueKindCompute,
    se_VkRequestedQueueFlagsTransfer = 0x1 << se_VkQueueKindTransfer,
    se_VkRequestedQueueFlagsSparseBinding = 0x1 << se_VkQueueKindSparseBinding,
    se_VkRequestedQueueFlagsPresent = 0x1 << se_VkQueueKindPresent,
} seVkRequestedQueueFlags;

/**
 * @brief Defines the current Vulkan specific capabilities of the graphics system.
 *
 */
typedef struct seVkGraphicsCapabilities
{
    seVkQueueFlags m_SupportedQueueTypes;
    se_uint32 m_MaxQueueCount;
} seVkGraphicsCapabilities;

/**
 * @brief TODO @david.mohrhardt
 *
 */
typedef struct seVkInstanceCreateArgs
{
    const char** m_ppEnabledLayers;
    se_uint32 m_EnabledLayerCount;

    const char** m_ppEnabledInstanceExtensions;
    se_uint32 m_EnabledInstanceExtensionCount;
} seVkInstanceCreateArgs;

/**
 * @brief Defines creation parameters for the Vulkan physical device.
 */
typedef struct seVkPhysicalDeviceCreateArgs
{
    /**
     * @brief The index of the preferred graphics device. This is can be
     * used to select a specific device if multiple devices are available
     * or reuse a device if the application is restarted. If the index
     * is out of range, the engine will run a heuristic to select the
     * best device.
     *
     */
    se_int8 m_PreferredGraphicsDeviceIndex;
} seVkPhysicalDeviceCreateArgs;

/**
 * @brief TODO @david.mohrhardt
 *
 */
typedef struct seVkLogicalDeviceCreateArgs
{

    /**
     * @brief The names of the device extensions that should be enabled.
     */
    const char** m_ppEnabledDeviceExtensions;

    /**
     * @brief The number of device extensions in the m_ppEnabledDeviceExtensions array.
     */
    se_uint32 m_EnabledDeviceExtensionCount;

    seVkQueueFlags m_RequestedQueueTypes;
    se_uint32 m_RequestedQueueCount;
    float* m_pQueuePriorities;
} seVkLogicalDeviceCreateArgs;

/**
 * @brief Provides the creation parameters for the Vulkan driver.
 *
 */
typedef struct seVkDriverCreateInfo
{
    seVkInstanceCreateArgs m_InstanceCreateArgs;
    seVkPhysicalDeviceCreateArgs m_PhysicalDeviceCreateArgs;
    seVkLogicalDeviceCreateArgs m_LogicalDeviceCreateArgs;

    bool m_EnableValidationLayers;

    bool m_RequestSurface;
    void* m_pWindowHandle;

    void* m_pNext;
    void* m_pUserData;
} seVkDriverCreateInfo;

#endif // !I_SAVANNA_GFX_VK2_H
