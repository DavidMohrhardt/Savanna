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
#include <assert.h>
#include <mutex>

#include <Utilities/Console.h>

namespace Savanna::Entities::ComponentRegistry
{
    using namespace Savanna::Entities;

    // TODO @DavidMohrhardt Create a SingleWriterMultipleReader lockless version of this
    std::mutex g_ComponentRegistryMutex;

    ComponentKey g_ComponentKeyCounter = ComponentKey(0x1);

    std::unordered_map<std::type_index, ComponentKey> s_ComponentTypeMap = {};

    const se_ComponentKeyMask_T GetNumberOfComponentKeySets()
    {
        std::lock_guard<std::mutex> lock(g_ComponentRegistryMutex);
        return g_ComponentKeyCounter.GetRingIndex();
    }

    const uint32 GetTotalNumberOfRegisteredComponents()
    {
        std::lock_guard<std::mutex> lock(g_ComponentRegistryMutex);
        return s_ComponentTypeMap.size();
    }

    const ComponentKey GetNextAvailableComponentKey()
    {
        std::lock_guard<std::mutex> lock(g_ComponentRegistryMutex);
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

    static inline void UpdateComponentKeyCounter()
    {
        se_ComponentKey_t nextComponentKey = g_ComponentKeyCounter.GetKeyValue() << 1;
        if ((nextComponentKey & k_ComponentKeyDataTeethMask) != 0x0)
        {
            g_ComponentKeyCounter.SetKeyValue(nextComponentKey);
        }
        else
        {
            g_ComponentKeyCounter.SetRingIndex(g_ComponentKeyCounter.GetRingIndex() + 1);
            g_ComponentKeyCounter.SetKeyValue(1);
        }
    }

    const ComponentKey RegisterComponentWithTypeIndex(const std::type_index typeIndex)
    {
        std::lock_guard<std::mutex> lock(g_ComponentRegistryMutex);
        ComponentKey componentId = SE_INVALID_HANDLE;
        if (s_ComponentTypeMap.find(typeIndex) == s_ComponentTypeMap.end())
        {
            SAVANNA_ASSERT(SavannaIsValidComponentKey(g_ComponentKeyCounter), "Component ID overflow");

            componentId = g_ComponentKeyCounter;
            s_ComponentTypeMap.emplace(typeIndex, componentId);

            UpdateComponentKeyCounter();
        }
        else
        {
            componentId = s_ComponentTypeMap[typeIndex];
        }

        return componentId;
    }

} // namespace  Savanna::Entities
