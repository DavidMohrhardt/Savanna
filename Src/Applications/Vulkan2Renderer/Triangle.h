#pragma once

#include <Gfx/Public/ISavannaGfx.h>

class TriangleShaderCreateJob;

struct TriangleMesh
{
private:
    static seGfxShader s_DefaultTriangleShaderHandles[2];

    static TriangleShaderCreateJob* CreateShaderJob(
        se_uint8 index,
        seGfxShaderStage stage,
        const seAllocatorInterface* pAllocatorInterface);

    static bool TryCreateDefaultShader(const seAllocatorInterface* pAllocatorInterface);

public:

    static TriangleMesh& GetDefaultMesh();

    float* m_pVertices;
    uint32_t* m_pIndices;
    uint32_t* m_pUvCoords;
    uint32_t m_VertexCount;
    uint32_t m_IndexCount;
    uint32_t m_UvCoordCount;
};

namespace gpu
{
    struct triangle
    {
        seGfxHandle vertexBufferHandle;
        seGfxHandle indexBufferHandle;
    };

    // triangle CreateTriangle(TriangleMesh& mesh)
    // {
    //     se_
    // }
}
