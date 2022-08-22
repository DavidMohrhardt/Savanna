/**
 * @file Component.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 */

#pragma once

#include "ComponentRegistry.h"
#include "IComponent.h"

namespace Savanna::Entities
{
    template<typename T>
    struct IComponentData : public IComponent
    {
    public:
        inline static const ComponentId& s_ComponentId = ComponentRegistry::RegisterComponentTypeForType(typeid(IComponentData<T>));

    public:
        IComponentData() = default;
        virtual ~IComponentData() = default;

        SAVANNA_NO_DISCARD virtual const ComponentId& GetComponentId() const override final
        {
            return s_ComponentId;
        }
    };
}
