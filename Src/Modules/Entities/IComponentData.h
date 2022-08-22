/**
 * @file Component.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 */

#pragma once

#include "Entity.h"
#include "IComponent.h"
#include "ComponentRegistry.h"

namespace Savanna::ECS
{
    template<typename T>
    struct IComponentData : public IComponent
    {
    public:
        inline static const ComponentId& s_ComponentId = ComponentRegistry::RegisterComponentTypeForType(typeid(IComponentData<T>));

    private:
        T m_Data;

    public:
        IComponentData() = default;
        virtual ~IComponentData() = default;

        SAVANNA_NO_DISCARD virtual const ComponentId& GetComponentId() const override final
        {
            return s_ComponentId;
        }
    };
}
