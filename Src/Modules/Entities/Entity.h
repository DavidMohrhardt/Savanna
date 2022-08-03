#pragma once
#include "Types/Primitive/PrimitiveTypes.h"

namespace Savanna
{
    struct Entity
    {
        int64 m_EntityHandle;
        intptr m_ComponentReferenceBag;
    };
} // namespace Savanna
