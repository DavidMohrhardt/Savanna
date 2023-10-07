#pragma once

#include "SavannaVk.h"

#include "VkRendererCreateInfo.h"
#include "VkContext.h"

namespace Savanna::Gfx::Vk2
{
    // class VkDisplaySurface
    // {
    // private:
    //     friend class VkDriver;
    //     VkDisplaySurface() = default;
    //     VkDisplaySurface(const VkInstance& instance);//, const Vk)
    //     ~VkDisplaySurface();

    //     VkSurfaceKHR GetSurface() const { return m_Surface; }
    //     operator VkSurfaceKHR () const { return m_Surface; }

    // public:
    //     VkDisplaySurface(const VkDisplaySurface& other) = delete;
    //     VkDisplaySurface(VkDisplaySurface&& other) = delete;

    //     VkDisplaySurface& operator=(const VkDisplaySurface& other) = delete;
    //     VkDisplaySurface& operator=(VkDisplaySurface&& other) = delete;

    // public:
    //     bool IsValid() const { return m_Surface != VK_NULL_HANDLE; }

    //     se_GfxDisplaySurfaceHandle_t GetHandle() const { return reinterpret_cast<se_GfxDisplaySurfaceHandle_t>(m_Surface); }
    //     operator se_GfxDisplaySurfaceHandle_t () const { return GetHandle(); }

    // private:
    //     VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
    // };
} // namespace Savanna::Gfx::Vk
