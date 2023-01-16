/**
 * @file WindowsVulkanDisplaySurface.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief Provides Windows specific Vulkan surface creation utilities.
 * @version 0.1
 * @date 2022-08-21
 *
 */
#pragma once

// Core Engine Includes
#include <SavannaEngine.h>

#if SAVANNA_WINDOWS

// Vulkan Includes
#include <vulkan/vulkan.h>

// Engine Module Includes
#include "VkSurfaceCreateInfos.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace Savanna::Gfx::Vk
{
    namespace Windows
    {
        /**
         * @brief Fills out a Vulkan surface create info structure with the necessary information to
         * create a Vulkan surface on Windows.
         *
         * @param hInstance The application's instance handle.
         * @param hwndHandle The application's window handle.
         * @param outSurfaceCreateInfo The address of the surface create info structure to fill out.
         */
        void FillOutSurfaceCreateInfo(
            HINSTANCE hInstance,
            HWND hwndHandle,
            SurfaceCreateInfo* outSurfaceCreateInfo);
    } // namespace Savanna::Gfx::Vk::Windows
} // namespace Savanna::Gfx::Vk

#endif // SAVANNA_WINDOWS
