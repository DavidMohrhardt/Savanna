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

#include "Public/ISavannaEntities.hpp"

#include "SavannaEngine.h"
#include "Utilities/SavannaCoding.h"

#include "Types/Locks/MultiReadSingleWriteLock.h"
#include "Types/Singleton/Singleton.h"

#include <atomic>
#include <typeindex>
#include <unordered_map>

namespace savanna::Entities
{
    class ComponentRegistry : public Singleton<ComponentRegistry>
    {
    private:
        DECLARE_FRIENDS_FOR_SINGLETON(ComponentRegistry);

        std::atomic<se_ComponentKey_t> m_NextAvailableComponentKey;
        MultiReadSingleWriteLock m_ComponentTypeMapLock;
        std::unordered_map<intptr, se_ComponentKey_t> m_ComponentTypeMap;

        ComponentRegistry() = default;
        ~ComponentRegistry() = default;

    private:
        const uint32 GetTotalNumberOfRegisteredComponentsInternal();
        // const se_ComponentKey_t RegisterComponent(const std::type_index& typeIndex);

    public:
        static const uint8 GetNumberOfComponentKeySets();
        static const uint32 GetTotalNumberOfRegisteredComponents();
        // static const se_ComponentKey_t RegisterComponent(const IComponent);
    };
} // namespace savanna::Entities
