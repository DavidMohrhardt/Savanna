#pragma once

#include "IComponentData.h"
#include <Types/Strings/CompileTimeHashString>

namespace savanna::Entities
{
    SAVANNA_DECLARE_ECS_COMPONENT_STRUCT(Tag)
    {
        HashString m_Tag;
    };
} // namespace savanna::Entities
