/**
 * @file Component.h
 * @author David Mohrhardt (DavidMohrhardt@github.com)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <Types/Primitive/PrimitiveTypes.h>

#include "Entity.h"

namespace Savanna::ECS
{
    struct Component
    {
    public:
        static uint32 m_ComponentId;

        static void RegisterComponent()
        {

        }

        void AddComponent(Entity entity)
        {
            // Find available entity block
        }
    };

    template<typename T>
    struct ComponentData
    {

    };
}
