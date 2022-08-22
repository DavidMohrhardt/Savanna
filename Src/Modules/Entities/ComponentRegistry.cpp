/**
 * @file ComponentManager.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-03
 *
 */

#include "ComponentManager.h"
#include "Component.h"

#include <mutex>

namespace Savanna::ECS
{
    // Mask the most significant 8 bits of the component ID to represent additional sets of components.
    // 8! * (54) = 2177280 possible unique component IDs.
    std::mutex g_ComponentRegistryMutex;
    volatile ComponentId g_ComponentIdCounter = { 0x1ull };

    const ComponentId& ComponentManager::GetComponentId(const IComponent* const componentPtr)
    {
        std::lock_guard<std::mutex> lock(g_ComponentRegistryMutex);
        return m_ComponentTypeMap[typeid(T)];
    }

    void ComponentManager::RegisterComponentType(const IComponent* const componentPtr)
    {
        std::lock_guard<std::mutex> lock(g_ComponentRegistryMutex);
        if (m_ComponentTypeMap.find(typeid(T)) == m_ComponentTypeMap.end())
        {
            assert(g_ComponentIdCounter.m_ComponentId != SAVANNA_INVALID_COMPONENT_ID && "Component ID overflow");
            m_ComponentTypeMap[typeid(T)] = g_ComponentIdCounter++;

            if (!se_IsValidComponentId(g_ComponentIdCounter))
            {
                m_MaxSetMask = ++g_ComponentIdCounter.m_SetMask;
            }
        }
    }

} // namespace Savanna::ECS

