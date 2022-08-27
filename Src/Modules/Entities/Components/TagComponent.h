#pragma once

#include "IComponentData.h"
#include <Types/Strings/HashString.h>

namespace Savanna::Entities
{
    struct TagComponent : public IComponentData<TagComponent>
    {
        HashString m_Tag;
    };
} // namespace Savanna::Entities
