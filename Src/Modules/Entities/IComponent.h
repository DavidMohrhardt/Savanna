/**
 * @file IComponent.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-21
 */
#pragma once

#include "ComponentId.h"

namespace Savanna::Entities
{
    struct ComponentIdFacade
    {
        ComponentId m_ComponentId;

        bool operator==(const ComponentIdFacade& other) const { return (*this == other); }
        bool operator!=(const ComponentIdFacade& other) const { return !(*this == other); }
        bool operator==(const ComponentId& other) const { return m_ComponentId.m_ComponentId == other.m_ComponentId; }
        bool operator!=(const ComponentId& other) const { return !operator==(other); }
    };

    struct IComponent
    {
    public:
        IComponent() {}

        virtual ~IComponent() = default;

        virtual const ComponentId& GetComponentId() = 0;
    };
} // namespace Savanna::Entities

