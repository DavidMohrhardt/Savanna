/**
 * @file Renderer.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-01-14
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>

#include "GfxDevice.h"

namespace Savanna::Rendering
{

    class Renderer
    {
    public:
        Renderer();
        virtual ~Renderer();

    public:
        // virtual CommandBuffer* AcquireCommandBuffer() const = 0;
        // virtual void ReleaseCommandBuffer(CommandBuffer* commandBuffer) const = 0;
        // virtual GfxDevice* GetGfxDevice() const = 0;
        // virtual void

    protected:
        virtual void UpdateImpl() = 0;
    }
} // namespace Savanna::Rendering
