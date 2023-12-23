#pragma once

#include "ECS.h"
#include "Public/SavannaECS.h"

#include <glm/Vector.hpp>

namespace savanna::Entities
{
    SAVANNA_DECLARE_ECS_COMPONENT_STRUCT(Transform)
    {
        glm::Vector3 m_Position;
        glm::Vector4 m_Rotation;
    };
} // namespace savanna::Entities
