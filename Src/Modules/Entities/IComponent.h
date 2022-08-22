/**
 * @file IComponent.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-21
 */
#pragma once

namespace Savanna::ECS
{
    struct IComponent
    {
    public:
        IComponent() {}

        virtual const ComponentId& GetComponentId() const
        {
            return ComponentManager::GetComponentType(GetTypeIndex());
        }

        virtual ~IComponent() = default;
        virtual const ComponentId& GetComponentId() const = 0;

    protected:
        constexpr std::type_index GetTypeIndex() const
        {
            return typeid(this);
        }
    };
} // namespace Savanna::ECS

