/**
 * @file ComponentRegistry.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-03
 *
 */

#include "ComponentRegistry.h"
#include "IComponent.h"

#include <cassert>
#include <mutex>

namespace Savanna::Entities
{
    // Mask the most significant 8 bits of the component ID to represent additional sets of components.
    // (2^8) * (64 - 8) = 14336 possible unique component IDs.
    // Additionally, making the mask 16 bits will result in (2^16) * (64 - 16) = 3145728 possible unique component IDs.
    std::mutex g_ComponentRegistryMutex;
    ComponentId g_ComponentIdCounter = { 0x1ull };

    std::unordered_map<std::type_index, se_ComponentId> ComponentRegistry::s_ComponentTypeMap;

    const ComponentId ComponentRegistry::GetComponentId(const IComponent* const componentPtr)
    {
        return GetComponentIdFromType(typeid(*componentPtr));
    }

    const ComponentId ComponentRegistry::RegisterComponentType(const IComponent* const componentPtr)
    {
        return RegisterComponentTypeForType(typeid(componentPtr));
    }


    const ComponentId ComponentRegistry::GetComponentIdFromType(const std::type_index typeIndex)
    {
        std::lock_guard<std::mutex> lock(g_ComponentRegistryMutex);
        if (s_ComponentTypeMap.find(typeIndex) == s_ComponentTypeMap.end())
        {
            return SAVANNA_INVALID_COMPONENT_ID;
        }

        return s_ComponentTypeMap[typeIndex];
    }

    const ComponentId ComponentRegistry::RegisterComponentTypeForType(const std::type_index typeIndex)
    {
        std::lock_guard<std::mutex> lock(g_ComponentRegistryMutex);
        ComponentId componentId = SAVANNA_INVALID_COMPONENT_ID;
        if (s_ComponentTypeMap.find(typeIndex) == s_ComponentTypeMap.end())
        {
            assert(!se_IsValidComponentId(g_ComponentIdCounter) && "Component ID overflow");
            componentId = g_ComponentIdCounter;
            s_ComponentTypeMap.emplace(typeIndex, componentId);
            g_ComponentIdCounter.m_ComponentId++;

            if (!se_IsValidComponentId(g_ComponentIdCounter))
            {
                // m_MaxSetMask = ++g_ComponentIdCounter.m_SetMask;
            }
        }
        else
        {
            componentId = s_ComponentTypeMap[typeIndex];
        }

        return componentId;
    }

} // namespace Savanna::Entities

