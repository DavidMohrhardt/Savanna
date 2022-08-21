/**
 * @file NativeVulkanDisplaySurfaceInfo.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-21
 *
 */
#pragma once

#include <Utilities/SavannaCoding.h>

#include <vulkan/vulkan.h>

#include "Windows/WindowsVulkanDisplaySurface.h"

namespace Savanna::Rendering::Vulkan
{
    enum VulkanSurfaceKind
    {
        Win32,
        Xlib,
        Xcb,
        Wayland,
        Mir,
        Android,
        Win32Khr,
        Macos,
        Ios,
    };

    typedef struct VulkanSurfaceCreateInfoUnion
    {
        union
        {
#if SAVANNA_WINDOWS
            VkWin32SurfaceCreateInfoKHR win32SurfaceCreateInfo;
#elif SAVANNA_LINUX || SAVANNA_DARWIN
            VkXlibSurfaceCreateInfoKHR xlibSurfaceCreateInfo;
            VkXcbSurfaceCreateInfoKHR xcbSurfaceCreateInfo;
            VkWaylandSurfaceCreateInfoKHR waylandSurfaceCreateInfo;
#elif SAVANNA_PLATFORM_ANDROID
            VkAndroidSurfaceCreateInfoKHR androidSurfaceCreateInfo;
            VkMirSurfaceCreateInfoKHR mirSurfaceCreateInfo;
#elif SAVANNA_PLATFORM_IOS
            VkIosSurfaceCreateInfoMVK iosSurfaceCreateInfo;
#endif
        };
        VulkanSurfaceKind m_SurfaceKind;
    } VulkanSurfaceCreateInfoUnion;
} // namespace Savanna::Rendering::Vulkan
