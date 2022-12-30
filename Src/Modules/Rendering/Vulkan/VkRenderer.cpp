/**
 * @file VkRenderer.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-20
 *
 */

#include "VkRenderer.h"

#include <Profiling/Profiler.h>

#include "Utilities/VkDefinitions.h"
#include "Utilities/VkRendererInitializationCache.h"

#include <vector>

namespace Savanna::Gfx::Vk
{
    /**
     * @brief Construct a new Vulkan Renderer:: Vulkan Renderer object
     *
     * @param pCreateInfo
     */
    Renderer::Renderer(const RendererCreateInfo* const pCreateInfo)
        : m_Context()
        , m_GfxDevice()
    {
        SAVANNA_INSERT_SCOPED_PROFILER(Renderer::Renderer(const RendererCreateInfo* const pCreateInfo));
        Initialize(pCreateInfo);
    }

    Renderer::Renderer(Renderer&& other)
    {
        *this = std::move(other);
    }

    Renderer::~Renderer()
    {
        SAVANNA_INSERT_SCOPED_PROFILER(Renderer::~Renderer());
    }

    Renderer& Renderer::operator=(Renderer&& other)
    {
        if (this != &other)
        {
            m_Context = std::move(other.m_Context);
            m_GfxDevice = std::move(other.m_GfxDevice);
        }
        return *this;
    }

    void Renderer::Initialize(const RendererCreateInfo* const pCreateInfo)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(Renderer::Initialize(const RendererCreateInfo* const pCreateInfo));
        Utils::RendererInitializationCache::Construct(pCreateInfo);

        SAVANNA_ASSERT(pCreateInfo != nullptr && "pCreateInfo is nullptr!");
        m_Context = Context(pCreateInfo);
        m_GfxDevice = GfxDevice(pCreateInfo, m_Context);

        Utils::RendererInitializationCache::Destroy();
    }
} // namespace Savanna::Gfx::Vk
