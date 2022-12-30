#pragma once

#include <SavannaEngine.h>

#ifndef SE_VK_RENDERER_CREATE_FLAGS_H
#define SE_VK_RENDERER_CREATE_FLAGS_H

#define SE_VK_ENABLE_VALIDATION_LAYERS_FLAG seVkRendererCreateFlagsEnableValidationLayers
#define SE_VK_ENABLE_DEBUG_MESSENGER_FLAG seVkRendererCreateFlagsEnableDebugMessenger
#define SE_VK_ENABLE_SURFACE_FLAG seVkRendererCreateFlagsEnableSurface
#define SE_VK_ENABLE_SWAPCHAIN_FLAG seVkRendererCreateFlagsEnableSwapchain
#define SE_VK_ENABLE_IMAGE_VIEWS_FLAG seVkRendererCreateFlagsEnableImageViews
#define SE_VK_ENABLE_RENDER_PASS_FLAG seVkRendererCreateFlagsEnableRenderPass
#define SE_VK_ENABLE_DESCRIPTOR_SET_LAYOUT_FLAG seVkRendererCreateFlagsEnableDescriptorSetLayout
#define SE_VK_ENABLE_GRAPHICS_PIPELINE_FLAG seVkRendererCreateFlagsEnableGraphicsPipeline
#define SE_VK_ENABLE_COMMAND_POOL_FLAG seVkRendererCreateFlagsEnableCommandPool
#define SE_VK_ENABLE_COLOR_RESOURCE_FLAG seVkRendererCreateFlagsEnableColorResource
#define SE_VK_ENABLE_DEPTH_RESOURCE_FLAG seVkRendererCreateFlagsEnableDepthResource
#define SE_VK_ENABLE_FRAMEBUFFERS_FLAG seVkRendererCreateFlagsEnableFramebuffers
#define SE_VK_ENABLE_ALL_FLAG seVkRendererCreateFlagsAll
#define SE_VK_ENABLE_NONE_FLAG seVkRendererCreateFlagsNone

#endif

DECLARE_SAVANNA_MODULE_FLAGS(
    Gfx::Vk, seVkRendererCreateFlags_t, RendererCreateFlags, __se_uint32,
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
