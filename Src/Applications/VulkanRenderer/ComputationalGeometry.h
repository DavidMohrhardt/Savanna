/**
 * @file ComputationalGeometry.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief Generates various geometric shapes.
 * @version 0.1
 * @date 2023-09-14
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <SavannaEngine.h>
#include <SavannaVk.h>

#include <VkRenderer.h>

#include <Types/Strings/FixedString.h>

namespace Savanna::Gfx::Vk
{
    namespace Shapes
    {
        void CreateDefaultShaderModules(Renderer& renderer);
    } // namespace Shapes

    class Triangle
    {
        static constexpr uint32_t VERTEX_BUFFER_BIND_ID = 0;
        static constexpr uint32_t VERTEX_BUFFER_BIND_COUNT = 1;
        static constexpr float32 k_Vertices[] = {
            0.0f, -0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f
        };

    public:
        Triangle();
        ~Triangle();
    };

    class Quad
    {
        static constexpr uint32_t VERTEX_BUFFER_BIND_ID = 0;

        static constexpr uint32_t VERTEX_BUFFER_BIND_COUNT = 1;

        static constexpr float32 k_Vertices[] {
            0.0f, 0.0f, 0.0f, // Top left
            1.0f, 0.0f, 0.0f, // Top right
            1.0f, 1.0f, 0.0f, // Bottom right
            0.0f, 1.0f, 0.0f  // Bottom left
        };

        static constexpr uint32_t k_Indices[] {
            0, 1, 2, // First triangle
            2, 3, 0  // Second triangle
        };

    public:
        Quad();
        ~Quad();

        void Draw();
    };
} // namespace Savanna::Gfx::Vk
