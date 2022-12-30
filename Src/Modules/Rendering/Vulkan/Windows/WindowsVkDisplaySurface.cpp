/**
 * @file WindowsVulkanDisplaySurface.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief Implements Windows specific Vulkan surface creation utilities.
 * @version 0.1
 * @date 2022-08-21
 *
 */
#include "WindowsVkDisplaySurface.h"
#include "VkSurfaceCreateInfos.h"

namespace Savanna::Gfx::Vk::Windows
{
    void FillOutSurfaceCreateInfo(
        HINSTANCE hInstance,
        HWND hwndHandle,
        SurfaceCreateInfo* surfaceCreateInfo)
    {
        surfaceCreateInfo->win32SurfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        surfaceCreateInfo->win32SurfaceCreateInfo.pNext = nullptr;
        surfaceCreateInfo->win32SurfaceCreateInfo.flags = 0;
        surfaceCreateInfo->win32SurfaceCreateInfo.hinstance = hInstance;
        surfaceCreateInfo->win32SurfaceCreateInfo.hwnd = hwndHandle;
    }
} // namespace Savanna::Gfx::Vk::Windows
