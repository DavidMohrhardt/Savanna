#pragma once

#include "ECS.h"

#include <glm/Vector.hpp>

namespace Savanna::Entities
{
    struct TransformComponent : public IComponent
    {
        glm::Vector3 m_Position;
        glm::Vector4 m_Rotation;
    };
} // namespace Savanna::Entities
