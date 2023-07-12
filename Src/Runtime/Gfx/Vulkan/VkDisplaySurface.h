#pragma once

#include "SavannaVk.h"

#include "VkRendererCreateInfo.h"
#include "VkContext.h"

namespace Savanna::Gfx::Vk
{
    class DisplaySurface
    {
    public:
        DisplaySurface();
        DisplaySurface(const RendererCreateInfo* const pCreateInfo, const Context& context);
        DisplaySurface(const DisplaySurface& other) = delete;
        DisplaySurface(DisplaySurface&& other);
        ~DisplaySurface();

    public:
        // VkSurfaceKHR operators
        operator VkSurfaceKHR() const { return m_Surface; }

        DisplaySurface& operator=(const DisplaySurface& other) = delete;
        DisplaySurface& operator=(DisplaySurface&& other);

        // Equivalence operators
        bool operator==(const DisplaySurface& other) const { return m_Surface == other.m_Surface; }
        bool operator!=(const DisplaySurface& other) const { return m_Surface != other.m_Surface; }
        bool operator==(const VkSurfaceKHR& other) const { return m_Surface == other; }
        bool operator!=(const VkSurfaceKHR& other) const { return m_Surface != other; }
        bool operator==(const VkSurfaceKHR* const other) const { return m_Surface == *other; }
        bool operator!=(const VkSurfaceKHR* const other) const { return m_Surface != *other; }

    public:
        VkSurfaceKHR GetSurface() const { return m_Surface; }
        bool IsValid() const { return m_Surface != VK_NULL_HANDLE; }

    private:
        VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
        VkInstance m_Instance = VK_NULL_HANDLE;
    };
} // namespace Savanna::Gfx::Vk
