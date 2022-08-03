#pragma once

#include "Entities/Entity.h"
#include "Types/Primitive/PrimitiveTypes.h"

namespace Savanna
{
    struct ComponentBase
    {
    public:
        uint32 m_ComponentId;

        static void RegisterComponent()
        {

        }

        void AddComponent(Entity entity)
        {
            // Find available entity block
        }
    };
}