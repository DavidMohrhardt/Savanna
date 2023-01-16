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

DECLARE_SAVANNA_MODULE_ENUM(Gfx::Vk, se_VkSurfaceKind_t, SurfaceKind, se_uint32,
    SE_VK_SURFACE_KIND_WIN32,
    SE_VK_SURFACE_KIND_MACOS,
    SE_VK_SURFACE_KIND_XLIB,
    SE_VK_SURFACE_KIND_XCB,
    SE_VK_SURFACE_KIND_WAYLAND,
    SE_VK_SURFACE_KIND_ANDROID,
    SE_VK_SURFACE_KIND_MIR,
    SE_VK_SURFACE_KIND_IOS
);

typedef struct se_VkSurfaceCreateInfo_t
{
    union
    {
#if SAVANNA_WINDOWS
        VkWin32SurfaceCreateInfoKHR win32SurfaceCreateInfo;
#elif SAVANNA_DARWIN
        VkMacOSSurfaceCreateInfoMVK macOSSurfaceCreateInfo;
#elif SAVANNA_LINUX
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
    se_VkSurfaceKind_t m_SurfaceKind;
} se_VkSurfaceCreateInfo_t;
DECLARE_SAVANNA_EXTENDED_NAMESPACED_CPP_TYPE_DEF(Gfx::Vk, se_VkSurfaceCreateInfo_t, SurfaceCreateInfo);
