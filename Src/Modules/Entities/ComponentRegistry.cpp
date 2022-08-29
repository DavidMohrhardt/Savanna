/**
 * @file ComponentRegistry.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/ Savanna)
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

//  Savanna includes
#if SAVANNA_TODO_TEST_LOCKLESS_COMPONENT_REGISTRY
#include <Types/Locks/VolatileLock.h>
#endif

#include <Utilities/Console.h>

namespace Savanna::Entities::ComponentRegistry
{
    using namespace Savanna::Entities;

    // TODO @DavidMohrhardt Consider moving to an atomic approach instead of a mutex.
#if SAVANNA_TODO_TEST_LOCKLESS_COMPONENT_REGISTRY
    VolatileLock g_ComponentKeyAccessLock;
#endif // end SAVANNA_TODO_TEST_LOCKLESS_COMPONENT_REGISTRY

    std::mutex g_ComponentRegistryMutex;

    ComponentKey g_ComponentKeyCounter = { .m_FullComponentKey = 0x1 };

    std::unordered_map<std::type_index, SavannaComponentKey> s_ComponentTypeMap = {};

    const se_ComponentKeyMask_T GetNumberOfComponentKeySets()
    {
        // TODO @DavidMohrhardt Perhaps can use a volatility lock instead of a mutex.
#if SAVANNA_TODO_TEST_LOCKLESS_COMPONENT_REGISTRY
        VolatileLockGuard lock(g_ComponentKeyAccessLock);
#else // Original
        std::lock_guard<std::mutex> lock(g_ComponentRegistryMutex);
#endif // end SAVANNA_TODO_TEST_LOCKLESS_COMPONENT_REGISTRY
        return g_ComponentKeyCounter.m_Set;
    }

    const uint32 GetTotalNumberOfRegisteredComponents()
    {
        // TODO @DavidMohrhardt Perhaps can use a volatility lock instead of a mutex.
#if SAVANNA_TODO_TEST_LOCKLESS_COMPONENT_REGISTRY
        VolatileLockGuard lock(g_ComponentKeyAccessLock);
#else // Original
        std::lock_guard<std::mutex> lock(g_ComponentRegistryMutex);
#endif // end SAVANNA_TODO_TEST_LOCKLESS_COMPONENT_REGISTRY
        return s_ComponentTypeMap.size();
    }

    const ComponentKey GetNextAvailableComponentKey()
    {
        // TODO @DavidMohrhardt Perhaps can use a volatility lock instead of a mutex.
#if SAVANNA_TODO_TEST_LOCKLESS_COMPONENT_REGISTRY
        VolatileLockGuard lock(g_ComponentKeyAccessLock);
#else // Original
        std::lock_guard<std::mutex> lock(g_ComponentRegistryMutex);
#endif // end SAVANNA_TODO_TEST_LOCKLESS_COMPONENT_REGISTRY
        return g_ComponentKeyCounter;
    }

    const ComponentKey GetComponentKey(const IComponent* const componentPtr)
    {
        return GetComponentKeyFromType(typeid(componentPtr));
    }

    const ComponentKey RegisterComponentType(const IComponent* const componentPtr)
    {
        return RegisterComponentWithTypeIndex(typeid(componentPtr));
    }

    const ComponentKey GetComponentKeyFromType(const std::type_index typeIndex)
    {
        std::lock_guard<std::mutex> lock(g_ComponentRegistryMutex);
        if (s_ComponentTypeMap.find(typeIndex) == s_ComponentTypeMap.end())
        {
            return SE_INVALID_HANDLE;
        }

        return s_ComponentTypeMap[typeIndex];
    }

    const ComponentKey RegisterComponentWithTypeIndex(const std::type_index typeIndex)
    {
        std::lock_guard<std::mutex> lock(g_ComponentRegistryMutex);
        ComponentKey componentId = SE_INVALID_HANDLE;
        auto ptr = &g_ComponentKeyCounter;
        if (s_ComponentTypeMap.find(typeIndex) == s_ComponentTypeMap.end())
        {
            assert(SavannaIsValidComponentKey(g_ComponentKeyCounter) && "Component ID overflow");

            componentId = g_ComponentKeyCounter;
            s_ComponentTypeMap.emplace(typeIndex, componentId);

            se_ComponentKey_t maskedKey = g_ComponentKeyCounter.m_Key;
            maskedKey <<= 1;

            if (maskedKey > SAVANNA_COMPONENT_KEY_MAX_VALUE)
            {
                g_ComponentKeyCounter.m_Key = 0x1;
                g_ComponentKeyCounter.m_Set++;
            }
            else
            {
                g_ComponentKeyCounter.m_Key <<= 1;
            }
        }
        else
        {
            componentId = s_ComponentTypeMap[typeIndex];
        }

        return componentId;
    }

} // namespace  Savanna::Entities
