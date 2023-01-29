#pragma once

#include <vulkan/vulkan.h>
#include <Types/Enums/Flags.h>

/**
 * @brief Defines a Vulkan queue flag enumeration.
 *
 */
DEFINE_SAVANNA_EXTERNAL_FLAG_ENUMERATION(Savanna::Gfx::Vk, QueueFlagBits, seVkQueueFlagBits, VkQueueFlagBits, uint32_t,
    seVkQueueFlagBitsNone = 0,
    seVkQueueFlagBitsAll = ~seVkQueueFlagBitsNone,
    seVkQueueFlagBitsGraphicsBit = VK_QUEUE_GRAPHICS_BIT,
    seVkQueueFlagBitsComputeBit = VK_QUEUE_COMPUTE_BIT,
    seVkQueueFlagBitsTransferBit = VK_QUEUE_TRANSFER_BIT,
    seVkQueueFlagBitsSparseBindingBit = VK_QUEUE_SPARSE_BINDING_BIT,
    seVkQueueFlagBitsProtectedBit = VK_QUEUE_PROTECTED_BIT,
    seVkQueueFlagBitsPresentBit = VK_QUEUE_PROTECTED_BIT << 1,
    seVkQueueFlagBitsMaxEnum = VK_QUEUE_FLAG_BITS_MAX_ENUM
);
