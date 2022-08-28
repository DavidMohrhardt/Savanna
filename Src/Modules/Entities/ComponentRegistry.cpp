/**
 * @file ComponentRegistry.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-03
 *
 */
#include "ComponentRegistry.h"

// Entities includes
#include <IComponent.h>
#include <IComponentData.h>

// Standard includes
#include <cassert>
#include <mutex>

// Savanna includes
#if SAVANNA_TODO_TEST_LOCKLESS_COMPONENT_REGISTRY
#include <Types/Locks/VolatileLock.h>
#endif

#include <Utilities/Console.h>

namespace Savanna::Entities::ComponentRegistry
{
    using namespace Savanna::Entities;

    constexpr uint8 k_ComponentIdMask = 0xFF;
    constexpr uint64 k_ComponentIdMask64 = 0x00FFFFFFFFFFFFFF;

    // TODO @DavidMohrhardt Consider moving to an atomic approach instead of a mutex.
#if SAVANNA_TODO_TEST_LOCKLESS_COMPONENT_REGISTRY
    VolatileLock g_ComponentIdAccessLock;
#else // Original
    std::mutex g_ComponentRegistryMutex;
#endif // end SAVANNA_TODO_TEST_LOCKLESS_COMPONENT_REGISTRY


    ComponentId g_ComponentIdCounter = { .m_ComponentId = 1 };

    std::unordered_map<std::type_index, SEComponentId> s_ComponentTypeMap = {};

    const uint8 GetNumberOfComponentIdSets()
    {
        // TODO @DavidMohrhardt Perhaps can use a volatility lock instead of a mutex.
#if SAVANNA_TODO_TEST_LOCKLESS_COMPONENT_REGISTRY
        VolatileLockGuard lock(g_ComponentIdAccessLock);
#else // Original
        std::lock_guard<std::mutex> lock(g_ComponentRegistryMutex);
#endif // end SAVANNA_TODO_TEST_LOCKLESS_COMPONENT_REGISTRY
        return g_ComponentIdCounter.m_Set;
    }

    const uint32 GetTotalNumberOfRegisteredComponents()
    {
        // TODO @DavidMohrhardt Perhaps can use a volatility lock instead of a mutex.
#if SAVANNA_TODO_TEST_LOCKLESS_COMPONENT_REGISTRY
        VolatileLockGuard lock(g_ComponentIdAccessLock);
#else // Original
        std::lock_guard<std::mutex> lock(g_ComponentRegistryMutex);
#endif // end SAVANNA_TODO_TEST_LOCKLESS_COMPONENT_REGISTRY
        return s_ComponentTypeMap.size();
    }

    const ComponentId GetNextAvailableComponentId()
    {
        // TODO @DavidMohrhardt Perhaps can use a volatility lock instead of a mutex.
#if SAVANNA_TODO_TEST_LOCKLESS_COMPONENT_REGISTRY
        VolatileLockGuard lock(g_ComponentIdAccessLock);
#else // Original
        std::lock_guard<std::mutex> lock(g_ComponentRegistryMutex);
#endif // end SAVANNA_TODO_TEST_LOCKLESS_COMPONENT_REGISTRY
        return g_ComponentIdCounter;
    }

    const ComponentId GetComponentId(const IComponent* const componentPtr)
    {
        return GetComponentIdFromType(typeid(componentPtr));
    }

    const ComponentId RegisterComponentType(const IComponent* const componentPtr)
    {
        return RegisterComponentWithTypeIndex(typeid(componentPtr));
    }

    const ComponentId GetComponentIdFromType(const std::type_index typeIndex)
    {
        std::lock_guard<std::mutex> lock(g_ComponentRegistryMutex);
        if (s_ComponentTypeMap.find(typeIndex) == s_ComponentTypeMap.end())
        {
            return SE_INVALID_HANDLE;
        }

        return s_ComponentTypeMap[typeIndex];
    }

    const ComponentId RegisterComponentWithTypeIndex(const std::type_index typeIndex)
    {
        std::lock_guard<std::mutex> lock(g_ComponentRegistryMutex);
        ComponentId componentId = SE_INVALID_HANDLE;
        auto ptr = &g_ComponentIdCounter;
        if (s_ComponentTypeMap.find(typeIndex) == s_ComponentTypeMap.end())
        {
            assert(se_IsValidComponentId(g_ComponentIdCounter) && "Component ID overflow");

            componentId = g_ComponentIdCounter;
            s_ComponentTypeMap.emplace(typeIndex, componentId);

#if SAVANNA_TODO_TEST_LOCKLESS_COMPONENT_REGISTRY
            VolatileLockGuard lock(g_ComponentIdAccessLock);
#endif // end SAVANNA_TODO_TEST_LOCKLESS_COMPONENT_REGISTRY

            g_ComponentIdCounter.m_ComponentKey = g_ComponentIdCounter.m_ComponentKey << 1;
            if (g_ComponentIdCounter.m_ComponentKey < componentId.m_ComponentKey)
            {
                g_ComponentIdCounter.m_Set++;
                g_ComponentIdCounter.m_ComponentKey = 0x1;
            }

        }
        else
        {
            componentId = s_ComponentTypeMap[typeIndex];
        }

        return componentId;
    }

} // namespace Savanna::Entities

