#pragma once

#include "IComponentData.h"
#include <Types/Strings/HashString.h>

namespace Savanna::Entities
{
    SAVANNA_DECLARE_ECS_COMPONENT(Tag)
    {
        HashString m_Tag;
    };
} // namespace Savanna::Entities
