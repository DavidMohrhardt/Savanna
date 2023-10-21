#include "VkDisplaySurface.h"

#include <Profiling/Profiler.h>

#if SAVANNA_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#endif // OS Select

namespace Savanna::Gfx::Vk
{
    DisplaySurface::DisplaySurface()
    {
    }

    DisplaySurface::DisplaySurface(const RendererCreateInfo *const pCreateInfo, const Context &context)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(DisplaySurface::DisplaySurface(const RendererCreateInfo *const pCreateInfo, const Context &context));
        SAVANNA_ASSERT(pCreateInfo != nullptr, "pCreateInfo is nullptr!");
        SAVANNA_ASSERT(context.IsValid(), "Context is not valid!");
        if (m_Surface != VK_NULL_HANDLE) SAVANNA_BRANCH_UNLIKELY
        {
            SAVANNA_WARNING_LOG("DisplaySurface is already initialized!");
            return;
        }

#if SAVANNA_WINDOWS
        VkResult result = vkCreateWin32SurfaceKHR(context, &pCreateInfo->m_SurfaceCreateInfo.win32SurfaceCreateInfo, nullptr, &m_Surface);
        if (result != VK_SUCCESS) SAVANNA_BRANCH_UNLIKELY
        {
            SAVANNA_WARNING_LOG("Vulkan surface is unsupported!");
            return;
        }
#endif
        m_Instance = context;
    }

    DisplaySurface::DisplaySurface(DisplaySurface &&other)
    {
        *this = std::move(other);
    }

    DisplaySurface::~DisplaySurface()
    {
        SAVANNA_INSERT_SCOPED_PROFILER(DisplaySurface::~DisplaySurface());
        if (m_Surface != VK_NULL_HANDLE && m_Instance != VK_NULL_HANDLE) SAVANNA_BRANCH_LIKELY
        {
            vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
            m_Surface = VK_NULL_HANDLE;
            m_Instance = VK_NULL_HANDLE;
        }
    }

    DisplaySurface &DisplaySurface::operator=(DisplaySurface &&other)
    {
        m_Surface = std::move(other.m_Surface);
        m_Instance = std::move(other.m_Instance);
        other.m_Surface = VK_NULL_HANDLE;
        other.m_Instance = VK_NULL_HANDLE;
        return *this;
    }
}
