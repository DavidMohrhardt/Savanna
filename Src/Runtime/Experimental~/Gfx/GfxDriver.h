/**
 * @file GfxDriver.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-09-15
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <SavannaEngine.h>

#include "Types/Memory/MemoryBuffer.h"

#include "GfxBuffer.h"

namespace Savanna::Gfx
{
    class IGfxDriver
    {
    public:
        virtual void Create() = 0;
        virtual void Destroy() = 0;
    };
} // namespace Savanna::Gfx
