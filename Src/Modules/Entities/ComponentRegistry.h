/**
 * @file ComponentManager.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-03
 *
 */

#pragma once

#include <SavannaEngine.h>
#include <SavannaCoding.h>

#undef SAVANNA_INVALID_COMPONENT_ID
#define SAVANNA_INVALID_COMPONENT_ID 0x0

namespace Savanna::ECS
{
    /**
     * @brief Provides a registry for all IComponentData types. Components are provided a unique identifier
     *       when they are registered.
     */
    class ComponentRegistry
    {
    private:
        ComponentRegistry() = delete;
        ~ComponentRegistry() = delete;

    private:
        static std::unordered_map<std::type_index, ComponentId> s_ComponentIds;

    public:
        static constexpr uint8 k_ComponentIdMask = 0xFF;
        static constexpr uint64 k_ComponentIdMask64 = 0x00FFFFFFFFFFFFFF;

    public:
        // Might be overkill here, maybe just use the templated IComponentData
        static const ComponentId& GetComponentId(const IComponent* const componentPtr);
        static ComponentId RegisterComponentType(const IComponent* const componentPtr);

        static const ComponentId& GetComponentIdFromType(const std::type_index const typeIndex);
        static ComponentId RegisterComponentTypeForType(const std::type_index const typeIndex);
    };
} // namespace Savanna::ECS
