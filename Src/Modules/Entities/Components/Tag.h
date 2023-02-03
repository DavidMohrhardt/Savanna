#pragma once

#include "IComponentData.h"
#include <Types/Strings/CRC32HashString>

namespace Savanna::Entities
{
    SAVANNA_DECLARE_ECS_COMPONENT_STRUCT(Tag)
    {
        HashString m_Tag;
    };
} // namespace Savanna::Entities
