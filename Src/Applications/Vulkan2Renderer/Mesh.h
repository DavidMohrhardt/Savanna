#pragma once

#include "Gfx/ISavannaGfx.h"

using namespace savanna;

class GpuMesh
{
public:
    se_GpuBufferHandle_t m_VertexBuffer;
    se_GpuBufferHandle_t m_IndexBuffer;
};
