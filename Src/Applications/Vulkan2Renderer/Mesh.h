#pragma once

#include "Gfx/ISavannaGfx.h"

using namespace savanna;

class GpuMesh
{
public:
    seGpuBufferHandle m_VertexBuffer;
    seGpuBufferHandle m_IndexBuffer;
};
