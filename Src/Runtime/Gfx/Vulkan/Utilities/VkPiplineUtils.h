/**
 * @file 
 * @author 
 * @brief
 * @version 0.1
 * @date 
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <VkRenderer.h>

#include "SavannaVk.h"

#include "VkQueueFamilyIndices.h"
#include "VkRendererCreateInfo.h"
#include "VkRendererCreateUtils.h"

namespace Savanna::Gfx::Vk::Utils 
{
    DynamicArray<VkPipelineShaderStageCreateInfo> CreateShaderStages(DynamicArray<SE_VkShaderModule>& modules);

}