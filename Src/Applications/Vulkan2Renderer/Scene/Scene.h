/**
 * @file Scene.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-10-30
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <Gfx/Public/ISavannaGfx.h>

#include <Types/Containers/Arrays/dynamic_array.h>

using namespace savanna;

struct MaterialRenderingData
{
    seGfxShader m_VertexShaderModuleHandle;
    seGfxShader m_FragmentShaderModuleHandle;
};

class Material
{
private:
    MaterialRenderingData m_ShaderModuleHandle;

public:
    Material(MaterialRenderingData renderingData);
    ~Material();
};

class Mesh
{
private:
    seGfxBuffer m_VertexBufferHandle;
    seGfxBuffer m_IndexBufferHandle;
    Material m_Material;
};
