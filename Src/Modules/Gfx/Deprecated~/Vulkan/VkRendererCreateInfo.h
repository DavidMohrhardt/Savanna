#pragma once

#include <SavannaVk.h>

#include <vulkan/vulkan.h>

typedef bool(*se_VkDeviceTypeScoringFunc_t)(const VkPhysicalDeviceType&, se_uint32& outScore);
typedef bool(*se_VkDeviceFeatureScoringFunc_t)(const VkPhysicalDeviceFeatures&, se_uint32& outScore);
typedef bool(*se_VkDeviceLimitsScoringFunc_t)(const VkPhysicalDeviceLimits&, se_uint32& outScore);
typedef VkResult (*se_VkSurfaceCreateFunc_t)(VkInstance& instance, void* pWindow, VkAllocationCallbacks* pAllocationCallbacks, VkSurfaceKHR* surface);

DECLARE_SAVANNA_MODULE_FLAGS(
    Gfx::Vulkan, se_VkRendererCreateFlags_t, VkRendererCreateFlags, se_uint32,
    se_VkRendererCreateFlagsNone = 0,
    se_VkRendererCreateFlagsEnableValidationLayers = 1 << 0,
    se_VkRendererCreateFlagsEnableDebugMessenger = 1 << 1,
    se_VkRendererCreateFlagsEnableSurface = 1 << 2,
    se_VkRendererCreateFlagsEnableSwapchain = 1 << 3,
    se_VkRendererCreateFlagsEnableImageViews = 1 << 4,
    se_VkRendererCreateFlagsEnableRenderPass = 1 << 5,
    se_VkRendererCreateFlagsEnableDescriptorSetLayout = 1 << 6,
    se_VkRendererCreateFlagsEnableGraphicsPipeline = 1 << 7,
    se_VkRendererCreateFlagsEnableCommandPool = 1 << 8,
    se_VkRendererCreateFlagsEnableColorResource = 1 << 9,
    se_VkRendererCreateFlagsEnableDepthResource = 1 << 10,
    se_VkRendererCreateFlagsEnableFramebuffers = 1 << 11,
    se_VkRendererCreateFlagsAll = ~0u,
    se_VkRendererCreateFlagsVkEnsure32Bit = 0x7FFFFFFF
);

DECLARE_SAVANNA_MODULE_FLAGS(
    Gfx::Vulkan, se_VkRendererQueueFlags_t, VkRendererQueueFlags, se_uint32,
    se_VkRendererQueueFlagsNone = 0,
    se_VkRendererQueueFlagsGraphics = 1 << 0,
    se_VkRendererQueueFlagsPresent = 1 << 1,
    se_VkRendererQueueFlagsCompute = 1 << 2,
    se_VkRendererQueueFlagsTransfer = 1 << 3,
    se_VkRendererQueueFlagsSparseBinding = 1 << 4,
    se_VkRendererQueueFlagsAll = ~0u,
    se_VkRendererQueueFlagsEnsure32Bit = 0x7FFFFFFF
);

typedef struct se_VkRendererSurfaceCreateInfo_t
{
    void* m_pWindow;
    se_VkSurfaceCreateFunc_t m_SurfaceCreateFunction;
} se_VkRendererSurfaceCreateInfo_t;

typedef struct se_VkRendererCreateInfo_t
{
    // FixedString64 m_ApplicationName;
    // FixedString64 m_EngineName;

    const void* m_pNext;

    union
    {
        se_uint32 m_CreateFlagsRaw;
        se_VkRendererCreateFlags_t m_CreateFlags;
    };

    union
    {
        se_uint32 m_QueueFlagsRaw;
        se_VkRendererQueueFlags_t m_QueueFlags;
    };

    const char** m_ppInstanceExtensions;
    se_uint32 m_InstanceExtensionCount;
    const char** m_ppValidationLayers;
    se_uint32 m_ValidationLayerCount;

    se_uint32 m_PhysicalDeviceId;
    const char** m_ppDeviceExtensions;
    se_uint32 m_DeviceExtensionCount;

    se_VkRendererSurfaceCreateInfo_t m_SurfaceCreateInfo;

    VkAllocationCallbacks* m_pAllocationCallbacks;

    const void* m_pUserData;
} se_VkRendererCreateInfo_t;
DECLARE_SAVANNA_EXTENDED_NAMESPACED_CPP_TYPE_DEF(Gfx::Vulkan, se_VkRendererCreateInfo_t, VkRendererCreateInfo);
