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

#include "SavannaEntities.h"

#include "ParadigmLayoutDescriptor.h"

// Core Includes
#include "Utilities/SavannaCoding.h"

#include <Math/MathHelpers.h>
#include <Types/Containers/Arrays/array_view.h>
#include <Types/Containers/Lists/concurrent_list.h>
#include <Types/Memory/MemoryBlocks.h>
#include <Types/Memory/MemoryBuffer.h>

namespace Savanna::Entities
{
#if !DEPRECATED_PARADIGM
    class Paradigm;

    class ParadigmList
    {
        concurrent_list<Paradigm*> m_Paradigms;
    };

    class Paradigm
    {
    private:
        static constexpr size_t k_DefaultSizes[] {
            sizeof(UnifiedPage1024KiB),
            sizeof(UnifiedPage2048KiB),
            sizeof(UnifiedPage4096KiB)
        };

        MemoryBuffer m_ParadigmBuffer;
        Paradigm* m_NextParadigm;

        const se_ComponentKey_t m_ComponentParadigmKey;
        ParadigmLayoutDescriptor m_ParadigmLayoutDescriptor;

    public:
        Paradigm(const se_ComponentKey_t& componentParadigmKey, const size_t& componentCount);

        Paradigm(const Paradigm& other) = delete;

        Paradigm(Paradigm&& other) = delete;

        ~Paradigm();
    };
#else
    struct CompoundComponentKey
    {
        se_ComponentKey_t m_ComponentKeys[4];

        inline bool operator==(const CompoundComponentKey& other) const
        {
            return m_ComponentKeys[0] == other.m_ComponentKeys[0]
                && m_ComponentKeys[1] == other.m_ComponentKeys[1]
                && m_ComponentKeys[2] == other.m_ComponentKeys[2]
                && m_ComponentKeys[3] == other.m_ComponentKeys[3];
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
         * @brief A descriptor of the memory layout of this paradigm.
         */
        ParadigmLayoutDescriptor m_ParadigmLayoutDescriptor;

        union
        {
            /**
             * @brief The component keys that define this paradigm.
             */
            se_ComponentKey_t m_ParadigmKeyChain[SAVANNA_ECS_MAX_COMPONENT_PARADIGM_KEYS];

            /**
             * @brief The component keys that define this paradigm.
             */
            CompoundComponentKey m_CompoundParadigmKeyChain[
                GetRequiredLengthToFillUnion(
                    sizeof(se_ComponentKey_t) * SAVANNA_ECS_MAX_COMPONENT_PARADIGM_KEYS,
                    sizeof(CompoundComponentKey)
                )
            ];
        };

    public:
        Paradigm(const void* pParadigmMemory, const size_t& paradigmMemorySize);

        Paradigm(const Paradigm& other);

        Paradigm(Paradigm&& other);

        ~Paradigm();

    private:
        void AddComponentToParadigmInternal(
            const size_t& size,
            const size_t& alignment,
            const se_ComponentKey_t& componentKey);

        /**
         * @brief Allocate memory for the paradigm.
         * @param paradigmSize The size of the paradigm.
         */
        void AllocateParadigm(size_t paradigmSize);

        SAVANNA_NO_DISCARD void* GetComponentData(
            const size_t& stride,
            const size_t& alignment,
            se_ComponentKey_t componentKey,
            size_t* outArrayLength);

        void UpdateParadigmLayout();

    public:
        array_view<se_ComponentKey_t> GetKeyChain() const;

    public:
        template <typename T>
        SAVANNA_NO_DISCARD array_view<IComponentData<T>*> GetComponents(size_t* pOutComponentArrayLength)
        {
            if (pOutComponentArrayLength == nullptr)
            {
                return nullptr;
            }

            se_ComponentKey_t componentKey = IComponentData<T>::GetKey();
            if (!SavannaCompareKeys(
                    componentKey,
                    m_ParadigmKeyChain[componentKey.GetRingIndex()]))
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
            se_ComponentKey_t key = IComponentData<T>::GetKey();

            AddComponentToParadigmInternal(dataSize, dataAlignment, key);
        }
    };
#endif
} // namespace Savanna::Entities
