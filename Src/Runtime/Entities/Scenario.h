/**
 * @file World.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-10-30
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "SavannaEntities.h"

#include "System.h"

#include <Types/Containers/Arrays/dynamic_array.h>

namespace Savanna::Entities
{
    class UpdateLoop
    {
    private:
        Scenario& m_Scenario;
        uint32 m_Priority;
        dynamic_array<uint32> m_SystemIndices;
        MemoryBuffer m_ComponentBatchMemorySpace;

    public:
        UpdateLoop() = default;
        ~UpdateLoop() = default;

        uint32 GetPriority() const noexcept;
        void SetPriority(uint32 priority) noexcept;

        void AddSystem(uint32 systemIndex) noexcept;
        void RemoveSystem(uint32 systemIndex) noexcept;

        void Update() noexcept;
    };

    class Scenario
    {
    private:
        friend class UpdateLoop;

        dynamic_array<Entity> m_ActiveEntities;
        dynamic_array<Paradigm> m_EntityParadigms;
        dynamic_array<System> m_Systems;
        dynamic_array<UpdateLoop> m_UpdateLoops;

    public:
        Scenario() = default;
        ~Scenario() = default;

        void RegisterEntities(void** pComponents, uint32 componentCount, const ComponentKey& keyDefinition) noexcept;

        void RegisterSystem(const System& system) noexcept;
        void RegisterUpdateLoop(const UpdateLoop& updateLoop) noexcept;

        void Update() noexcept
        {
            for (auto& updateLoop : m_UpdateLoops)
            {
                updateLoop.Update();
            }
        }
    };
} // namespace Savanna::Entities
