/**
 * @file WindowsVulkanDisplaySurface.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-21
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include <SavannaEngine.h>

#if SAVANNA_WINDOWS

#include <vulkan/vulkan.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace Savanna::Rendering::Vulkan
{
    struct VulkanSurfaceCreateInfoUnion;

    namespace Windows
    {
        void FillOutSurfaceCreateInfo(HINSTANCE hInstance, HWND hwndHandle, VulkanSurfaceCreateInfoUnion* outSurfaceCreateInfo);
    } // namespace Savanna::Rendering::Vulkan::Windows
} // namespace Savanna::Rendering::Vulkan

#endif // SAVANNA_WINDOWS
