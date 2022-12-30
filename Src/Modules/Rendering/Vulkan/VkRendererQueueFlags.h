#pragma once

#include <SavannaEngine.h>

#define SV_VK_QUEUE_GRAPHICS_BIT seVkRendererQueueFlagsGraphics
#define SV_VK_QUEUE_PRESENT_BIT seVkRendererQueueFlagsPresent
#define SV_VK_QUEUE_COMPUTE_BIT seVkRendererQueueFlagsCompute
#define SV_VK_QUEUE_TRANSFER_BIT seVkRendererQueueFlagsTransfer
#define SV_VK_QUEUE_SPARSE_BINDING_BIT seVkRendererQueueFlagsSparseBinding
#define SV_VK_QUEUE_ALL_BITS seVkRendererQueueFlagsAll
#define SV_VK_QUEUE_NO_BITS seVkRendererQueueFlagsNone

DECLARE_SAVANNA_MODULE_FLAGS(
    Gfx::Vk, seVkRendererQueueFlags_t, RendererQueueFlags, __se_uint32,
    seVkRendererQueueFlagsNone = 0,
    seVkRendererQueueFlagsGraphics = 1 << 0,
    seVkRendererQueueFlagsPresent = 1 << 1,
    seVkRendererQueueFlagsCompute = 1 << 2,
    seVkRendererQueueFlagsTransfer = 1 << 3,
    seVkRendererQueueFlagsSparseBinding = 1 << 4,
    seVkRendererQueueFlagsAll = ~0u,
    seVkRendererQueueFlagsEnsure32Bit = 0x7FFFFFFF
);


