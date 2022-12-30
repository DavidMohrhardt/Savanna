#pragma once

#include <SavannaVk.h>

#include <vulkan/vulkan.h>

typedef bool(*seVkDeviceTypeScoringFunc_t)(const VkPhysicalDeviceType&, __se_uint32& outScore);
typedef bool(*seVkDeviceFeatureScoringFunc_t)(const VkPhysicalDeviceFeatures&, __se_uint32& outScore);
typedef bool(*seVkDeviceLimitsScoringFunc_t)(const VkPhysicalDeviceLimits&, __se_uint32& outScore);
typedef VkResult (*seVkSurfaceCreateFunc_t)(VkInstance& instance, void* pWindow, VkAllocationCallbacks* pAllocationCallbacks, VkSurfaceKHR* surface);

DECLARE_SAVANNA_MODULE_FLAGS(
    Gfx::Vulkan, seVkRendererCreateFlags_t, VkRendererCreateFlags, __se_uint32,
    seVkRendererCreateFlagsNone = 0,
    seVkRendererCreateFlagsEnableValidationLayers = 1 << 0,
    seVkRendererCreateFlagsEnableDebugMessenger = 1 << 1,
    seVkRendererCreateFlagsEnableSurface = 1 << 2,
    seVkRendererCreateFlagsEnableSwapchain = 1 << 3,
    seVkRendererCreateFlagsEnableImageViews = 1 << 4,
    seVkRendererCreateFlagsEnableRenderPass = 1 << 5,
    seVkRendererCreateFlagsEnableDescriptorSetLayout = 1 << 6,
    seVkRendererCreateFlagsEnableGraphicsPipeline = 1 << 7,
    seVkRendererCreateFlagsEnableCommandPool = 1 << 8,
    seVkRendererCreateFlagsEnableColorResource = 1 << 9,
    seVkRendererCreateFlagsEnableDepthResource = 1 << 10,
    seVkRendererCreateFlagsEnableFramebuffers = 1 << 11,
    seVkRendererCreateFlagsAll = ~0u,
    seVkRendererCreateFlagsVkEnsure32Bit = 0x7FFFFFFF
);

DECLARE_SAVANNA_MODULE_FLAGS(
    Gfx::Vulkan, seVkRendererQueueFlags_t, VkRendererQueueFlags, __se_uint32,
    seVkRendererQueueFlagsNone = 0,
    seVkRendererQueueFlagsGraphics = 1 << 0,
    seVkRendererQueueFlagsPresent = 1 << 1,
    seVkRendererQueueFlagsCompute = 1 << 2,
    seVkRendererQueueFlagsTransfer = 1 << 3,
    seVkRendererQueueFlagsSparseBinding = 1 << 4,
    seVkRendererQueueFlagsAll = ~0u,
    seVkRendererQueueFlagsEnsure32Bit = 0x7FFFFFFF
);

typedef struct seVkRendererSurfaceCreateInfo_t
{
    void* m_pWindow;
    seVkSurfaceCreateFunc_t m_SurfaceCreateFunction;
} seVkRendererSurfaceCreateInfo_t;

typedef struct seVkRendererCreateInfo_t
{
    // FixedString64 m_ApplicationName;
    // FixedString64 m_EngineName;

    const void* m_pNext;

    union
    {
        __se_uint32 m_CreateFlagsRaw;
        seVkRendererCreateFlags_t m_CreateFlags;
    };

    union
    {
        __se_uint32 m_QueueFlagsRaw;
        seVkRendererQueueFlags_t m_QueueFlags;
    };

    const char** m_ppInstanceExtensions;
    __se_uint32 m_InstanceExtensionCount;
    const char** m_ppValidationLayers;
    __se_uint32 m_ValidationLayerCount;

    __se_uint32 m_PhysicalDeviceId;
    const char** m_ppDeviceExtensions;
    __se_uint32 m_DeviceExtensionCount;

    seVkRendererSurfaceCreateInfo_t m_SurfaceCreateInfo;

    VkAllocationCallbacks* m_pAllocationCallbacks;

    const void* m_pUserData;
} seVkRendererCreateInfo_t;
DECLARE_SAVANNA_EXTENDED_NAMESPACED_CPP_TYPE_DEF(Gfx::Vulkan, seVkRendererCreateInfo_t, VkRendererCreateInfo);
