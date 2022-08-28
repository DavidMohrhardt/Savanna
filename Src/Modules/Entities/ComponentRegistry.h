/**
 * @file ComponentManager.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief A registry for both core and user defined ECS components. Purely a static class, it provides
 * tracking for the total number of components and generates unique IDs for each component type.
 * @version 0.1
 * @date 2022-08-03
 *
 */

#pragma once

#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>

#include "ComponentId.h"

#include <typeindex>
#include <unordered_map>

namespace Savanna::Entities::ComponentRegistry
{
    class IComponent;
    template<typename T> class IComponentData;

    const uint8 GetNumberOfComponentIdSets();
    const uint32 GetTotalNumberOfRegisteredComponents();
    const ComponentId GetNextAvailableComponentId();

    // Might be overkill here, maybe just use the templated IComponentData
    const ComponentId GetComponentId(const IComponent* const componentPtr);
    const ComponentId RegisterComponentType(const IComponent* const componentPtr);

    const ComponentId GetComponentIdFromType(const std::type_index typeIndex);
    const ComponentId RegisterComponentWithTypeIndex(const std::type_index typeIndex);

    template<typename T, class TComponentData = IComponentData<T>>
    const ComponentId GetComponentIdFromType()
    {
        static_assert(std::is_base_of<IComponentData<T>, TComponentData>::value, "TComponentData must be a subclass of IComponentData<T>");
        return GetComponentIdFromType(typeid(TComponentData));
    }

    template<typename T, class TComponentData = IComponentData<T>>
    const ComponentId RegisterComponentWithTypeIndex()
    {
        static_assert(std::is_base_of<IComponentData<T>, TComponentData>::value, "TComponentData must be a subclass of IComponentData<T>");
        return RegisterComponentWithTypeIndex(typeid(TComponentData));
    }
} // namespace Savanna::Entities
