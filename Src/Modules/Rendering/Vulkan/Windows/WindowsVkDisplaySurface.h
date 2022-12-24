/**
 * @file WindowsVkDisplaySurface.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief Provides Windows specific Vk surface creation utilities.
 * @version 0.1
 * @date 2022-08-21
 *
 */
#pragma once

#include <SavannaEngine.h>

#if SAVANNA_WINDOWS

#include <vulkan/vulkan.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace Savanna::Rendering::Vk
{
    struct VkSurfaceCreateInfoUnion;

    namespace Windows
    {
        /**
         * @brief Fills out a Vk surface create info structure with the necessary information to
         * create a Vk surface on Windows.
         *
         * @param hInstance The application's instance handle.
         * @param hwndHandle The application's window handle.
         * @param outSurfaceCreateInfo The address of the surface create info structure to fill out.
         */
        void FillOutSurfaceCreateInfo(
            HINSTANCE hInstance,
            HWND hwndHandle,
            VkSurfaceCreateInfoUnion* outSurfaceCreateInfo);
    } // namespace Savanna::Rendering::Vk::Windows
} // namespace Savanna::Rendering::Vk

#endif // SAVANNA_WINDOWS
