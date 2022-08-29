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
    struct ComponentKeyFacade
    {
        ComponentKey m_ComponentKey;

        bool operator==(const ComponentKeyFacade& other) const { return (*this == other); }
        bool operator!=(const ComponentKeyFacade& other) const { return !(*this == other); }
        bool operator==(const ComponentKey& other) const { return m_ComponentKey.m_FullComponentKey == other.m_FullComponentKey; }
        bool operator!=(const ComponentKey& other) const { return !operator==(other); }
    };

    struct IComponent
    {
    public:
        IComponent() {}

        virtual ~IComponent() = default;

        virtual const ComponentKey& GetComponentKey() = 0;
    };
} // namespace Savanna::Entities

