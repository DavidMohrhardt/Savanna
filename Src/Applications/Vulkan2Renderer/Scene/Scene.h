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

using namespace Savanna;

struct MaterialRenderingData
{
    se_GfxShaderHandle_t m_VertexShaderModuleHandle;
    se_GfxShaderHandle_t m_FragmentShaderModuleHandle;
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
    se_GfxBufferHandle_t m_VertexBufferHandle;
    se_GfxBufferHandle_t m_IndexBufferHandle;
    Material m_Material;
};
