#pragma once

#include "Gfx/ISavannaGfx.h"

using namespace Savanna;

class GpuMesh
{
public:
    se_GpuBufferHandle_t m_VertexBuffer;
    se_GpuBufferHandle_t m_IndexBuffer_t;
};
