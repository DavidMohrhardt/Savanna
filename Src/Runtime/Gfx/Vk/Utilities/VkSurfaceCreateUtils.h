/**
 * @file VkSurfaceCreateUtils.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-10-14
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "SavannaVk2.h"

namespace savanna::gfx::vk::utils
{
    VkSurfaceKHR CreateSurface(const VkInstance& instance, void* pWindowHandle, const VkAllocationCallbacks* pAllocCallbacks);
} // namespace savanna::Gfx::vk::Utils
