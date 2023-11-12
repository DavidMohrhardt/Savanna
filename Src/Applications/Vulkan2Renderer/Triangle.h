#pragma once

#include <Gfx/Public/ISavannaGfx.h>

class TriangleShaderCreateJob;

struct TriangleMesh
{
private:
    static se_GfxShaderHandle_t s_DefaultTriangleShaderHandles[2];

    static TriangleShaderCreateJob* CreateShaderJob(
        se_uint8 index,
        se_GfxShaderStage_t stage,
        const se_AllocatorInterface_t* pAllocatorInterface);

    static bool TryCreateDefaultShader(const se_AllocatorInterface_t* pAllocatorInterface);

public:

    static TriangleMesh& GetDefaultMesh();

    float* m_pVertices;
    uint32_t* m_pIndices;
    uint32_t* m_pUvCoords;
    uint32_t m_VertexCount;
    uint32_t m_IndexCount;
    uint32_t m_UvCoordCount;
};
