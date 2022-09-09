/**
 * @file IComponent.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-21
 */
#pragma once

#include "ComponentKey.h"

namespace Savanna::Entities
{
    struct IComponent
    {
    public:
        IComponent() {}

        virtual ~IComponent() = default;

        virtual const ComponentKey& GetComponentKey() = 0;
    };
} // namespace Savanna::Entities

