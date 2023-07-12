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

#include "ComponentKey.h"

#include <typeindex>
#include <unordered_map>

namespace Savanna::Entities::ComponentRegistry
{
    class IComponent;
    template<typename T> class IComponentData;

    const uint8 GetNumberOfComponentKeySets();
    const uint32 GetTotalNumberOfRegisteredComponents();
    const ComponentKey GetNextAvailableComponentKey();

    // Might be overkill here, maybe just use the templated IComponentData
    const ComponentKey GetComponentKey(const IComponent* const componentPtr);
    const ComponentKey RegisterComponentType(const IComponent* const componentPtr);

    const ComponentKey GetComponentKeyFromType(const std::type_index typeIndex);
    const ComponentKey RegisterComponentWithTypeIndex(const std::type_index typeIndex);

    template<typename T, class TComponentData = IComponentData<T>>
    const ComponentKey GetComponentKeyFromType()
    {
        static_assert(std::is_base_of<IComponentData<T>, TComponentData>::value, "TComponentData must be a subclass of IComponentData<T>");
        return GetComponentKeyFromType(typeid(TComponentData));
    }

    template<typename T, class TComponentData = IComponentData<T>>
    const ComponentKey RegisterComponentWithTypeIndex()
    {
        static_assert(std::is_base_of<IComponentData<T>, TComponentData>::value, "TComponentData must be a subclass of IComponentData<T>");
        return RegisterComponentWithTypeIndex(typeid(TComponentData));
    }
} // namespace Savanna::Entities
