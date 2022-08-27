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
    std::mutex g_ComponentRegistryMutex;
    ComponentId g_ComponentIdCounter = { 0x1ull };

    std::unordered_map<std::type_index, SEComponentId> ComponentRegistry::s_ComponentTypeMap = {};

    const uint8 ComponentRegistry::GetNumberOfComponentIdSets()
    {
        std::lock_guard<std::mutex> lock(g_ComponentRegistryMutex);
        return g_ComponentIdCounter.m_SetMask;
    }

    const uint32 ComponentRegistry::GetTotalNumberOfRegisteredComponents()
    {
        std::lock_guard<std::mutex> lock(g_ComponentRegistryMutex);
        return s_ComponentTypeMap.size();
    }

    const ComponentId ComponentRegistry::GetNextAvailableComponentId()
    {
        std::lock_guard<std::mutex> lock(g_ComponentRegistryMutex);
        return g_ComponentIdCounter;
    }

    const ComponentId ComponentRegistry::GetComponentId(const IComponent* const componentPtr)
    {
        return GetComponentIdFromType(typeid(*componentPtr));
    }

    const ComponentId ComponentRegistry::RegisterComponentType(const IComponent* const componentPtr)
    {
        return RegisterComponentWithTypeIndex(typeid(componentPtr));
    }

    const ComponentId ComponentRegistry::GetComponentIdFromType(const std::type_index typeIndex)
    {
        std::lock_guard<std::mutex> lock(g_ComponentRegistryMutex);
        if (s_ComponentTypeMap.find(typeIndex) == s_ComponentTypeMap.end())
        {
            return SE_INVALID_ID_HANDLE;
        }

        return s_ComponentTypeMap[typeIndex];
    }

    const ComponentId ComponentRegistry::RegisterComponentWithTypeIndex(const std::type_index typeIndex)
    {
        std::lock_guard<std::mutex> lock(g_ComponentRegistryMutex);
        ComponentId componentId = SE_INVALID_ID_HANDLE;
        if (s_ComponentTypeMap.find(typeIndex) == s_ComponentTypeMap.end())
        {
            assert(se_IsValidComponentId(g_ComponentIdCounter) && "Component ID overflow");
            componentId = g_ComponentIdCounter;
            s_ComponentTypeMap.emplace(typeIndex, componentId);
            // Shift the component ID and keep the set mask the same.
            g_ComponentIdCounter.m_ComponentId = (g_ComponentIdCounter.m_ComponentId & k_ComponentIdMaskSetMask) | (g_ComponentIdCounter.m_ComponentId & k_ComponentIdMask) << 1;

            if (se_GetBaseId(g_ComponentIdCounter) == SE_INVALID_ID)
            {
                // Id can now be 0 only 0x0ull is reserved for an invalid mask. Mask out the counter bits but keep the set mask.
                g_ComponentIdCounter.m_SetMask++;
            }
        }
        else
        {
            componentId = s_ComponentTypeMap[typeIndex];
        }

        return componentId;
    }

} // namespace Savanna::Entities

