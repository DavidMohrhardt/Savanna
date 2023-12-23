/**
 * @file VkSurfaceCreateUtils.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-10-14
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "VkSurfaceCreateUtils.h"

#if defined(SAVANNA_WINDOWS)
#include <Windows.h>
#include <vulkan/vulkan_win32.h>

#define VK_PLATFORM_WINDOW_HANDLE HWND

#elif defined(SAVANNA_LINUX)
// #include <X11/Xlib.h>
// #include <X11/Xutil.h>
// #include <vulkan/vulkan_xlib.h>

// #define VK_PLATFORM_WINDOW_HANDLE Window

#endif

namespace savanna::Gfx::Vk2::Utils
{
    VkSurfaceKHR CreateSurface(const VkInstance& instance, void* pWindowHandle, const VkAllocationCallbacks* pAllocCallbacks)
    {
        VkSurfaceKHR surface = VK_NULL_HANDLE;
        VK_PLATFORM_WINDOW_HANDLE windowHandle = (VK_PLATFORM_WINDOW_HANDLE)pWindowHandle;

#if defined(SAVANNA_WINDOWS)
        VkWin32SurfaceCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        createInfo.hinstance = GetModuleHandle(nullptr);
        createInfo.hwnd = windowHandle;

        auto vkCreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)vkGetInstanceProcAddr(instance, "vkCreateWin32SurfaceKHR");
        if (vkCreateWin32SurfaceKHR == nullptr || vkCreateWin32SurfaceKHR(instance, &createInfo, pAllocCallbacks, &surface) != VK_SUCCESS)
        {
            return VK_NULL_HANDLE;
        }
#elif defined(SAVANNA_PLATFORM_LINUX)
        #error "TODO"
#endif

        return surface;
    }
} // namespace savanna::Gfx::Vk2::Utils
