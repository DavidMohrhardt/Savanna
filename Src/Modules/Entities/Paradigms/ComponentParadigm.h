/**
 * @file Paradigm.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief Defines a component paradigm for the Savanna ECS.
 * A component paradigm is a set of component types that can be used to define an entity.
 *
 * @version 0.1
 * @date 2022-08-28
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include "Entities.h"

// Core Includes
#include <Math/MathHelpers.h>
#include <Memory/MemoryBlocks.h>
#include <Types/Containers/Arrays/ArraySlice.h>
#include <Utilities/SavannaCoding.h>


namespace Savanna::Entities
{
    struct CompoundComponentKey
    {
        ComponentKey m_ComponentKeys[4];

        inline bool operator==(const CompoundComponentKey& other) const
        {
            return m_ComponentKeys[0].m_FullComponentKey == other.m_ComponentKeys[0].m_FullComponentKey
                && m_ComponentKeys[1].m_FullComponentKey == other.m_ComponentKeys[1].m_FullComponentKey
                && m_ComponentKeys[2].m_FullComponentKey == other.m_ComponentKeys[2].m_FullComponentKey
                && m_ComponentKeys[3].m_FullComponentKey == other.m_ComponentKeys[3].m_FullComponentKey;
        }

        inline bool operator!=(const CompoundComponentKey& other) const
        {
            return !(*this == other);
        }

        inline CompoundComponentKey& operator=(const CompoundComponentKey& other)
        {
            m_ComponentKeys[0] = other.m_ComponentKeys[0];
            m_ComponentKeys[1] = other.m_ComponentKeys[1];
            m_ComponentKeys[2] = other.m_ComponentKeys[2];
            m_ComponentKeys[3] = other.m_ComponentKeys[3];
            return *this;
        }
    };

    class Paradigm
    {
    private:

        /**
         * @brief The memory pool that holds the component data for this paradigm.
         */
        const void* m_ParadigmMemory = nullptr;

        /**
         * @brief The total available memory of this paradigm paradigm.
         */
        size_t m_ParadigmMemorySize = 0;

        /**
         * @brief The full size of an entity that fulfills this paradigm.
         */
        size_t m_EntityParadigmSize = 0;

        /**
         * @brief The number of entity component sets stored in this paradigm.
         */
        uint32 m_EntityCount = 0;

        /**
         * @brief All arrays are aligned to the largest component alignment.
         *
         * TODO @DavidMohrhardt Maybe store metadata about the component in the paradigm to recompute the actual alighment of the component.
         */
        size_t m_LargestComponentAlignment = 0;

        union
        {
            /**
             * @brief The component keys that define this paradigm.
             */
            ComponentKey m_ParadigmKeyChain[SAVANNA_ECS_MAX_COMPONENT_PARADIGM_KEYS];

            /**
             * @brief The component keys that define this paradigm.
             */
            CompoundComponentKey m_CompoundParadigmKeyChain[
                GetRequiredLengthToFillUnion(
                    sizeof(ComponentKey) * SAVANNA_ECS_MAX_COMPONENT_PARADIGM_KEYS,
                    sizeof(CompoundComponentKey)
                )
            ];
        };

    public:
        Paradigm(const void* pParadigmMemory, const size_t& paradigmMemorySize);

        Paradigm() = default;
        Paradigm(const Paradigm&) = default;
        Paradigm(Paradigm&&) = default;

        ~Paradigm() = default;
    private:
        void AddComponentToParadigmInternal(
            const size_t& size,
            const size_t& alignment,
            const ComponentKey& componentKey);

        /**
         * @brief Allocate memory for the paradigm.
         * @param paradigmSize The size of the paradigm.
         */
        void AllocateParadigm(size_t paradigmSize);

        SAVANNA_NO_DISCARD void* GetComponentData(
            const size_t& stride,
            const size_t& alignment,
            ComponentKey componentKey,
            size_t* outArrayLength);

    public:
        ArraySlice<ComponentKey> GetKeyChain() const;

    public:
        // TODO @DavidMohrhardt Return a proper ArrayView instead of a pointer.
        template <typename T>
        SAVANNA_NO_DISCARD ArraySlice<IComponentData<T>*> GetComponents(size_t* pOutComponentArrayLength)
        {
            if (pOutComponentArrayLength == nullptr)
            {
                return nullptr;
            }

            ComponentKey componentKey = IComponentData<T>::GetKey();
            if (!SECompareKeys(componentKey, m_ParadigmKeyChain[componentKey.m_Set]))
            {
                // This component is not in this paradigm
                return nullptr;
            }

            return reinterpret_cast<IComponentData<T>*>(GetComponentData(sizeof(T), alignof(T), componentKey, pOutComponentArrayLength));
        }

        template <typename T, IComponentData<T> dataType>
        void AddComponentToParadigm()
        {
            size_t dataSize = sizeof(T);
            size_t dataAlignment = alignof(T);
            ComponentKey key = IComponentData<T>::GetKey();

            AddComponentToParadigmInternal(dataSize, dataAlignment, key);
        }
    };
} // namespace Savanna::Entities
