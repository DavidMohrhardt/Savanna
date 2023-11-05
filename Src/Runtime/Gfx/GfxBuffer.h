/**
 * @file GfxBuffer.h
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

namespace Savanna::Gfx
{
    class Buffer
    {
    public:
        Buffer();
        ~Buffer();

        void Create();
        void Destroy();

        void FlushData(bool invalidateCpuBuffer = false);

    private:
        uint32 m_BufferId;
        MemoryBuffer m_CpuBuffer;
        void* m_GpuBufferHandle;
    };
} // namespace Savanna::Gfx
