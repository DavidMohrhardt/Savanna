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

#include "ComponentKey.h"
#include "ParadigmLayoutDescriptor.h"

// Core Includes
#include "Utilities/SavannaCoding.h"

#include <Math/MathHelpers.h>
#include <Types/Containers/Arrays/ArraySlice.h>
#include <Types/Memory/MemoryBlocks.h>


namespace Savanna::Entities
{
    struct CompoundComponentKey
    {
        ComponentKey m_ComponentKeys[4];

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

        // Paradigm() = default;

        Paradigm(const Paradigm& other);

        Paradigm(Paradigm&& other);

        ~Paradigm();

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

        void UpdateParadigmLayout();

    public:
        ArraySlice<ComponentKey> GetKeyChain() const;

    public:
        template <typename T>
        SAVANNA_NO_DISCARD ArraySlice<IComponentData<T>*> GetComponents(size_t* pOutComponentArrayLength)
        {
            if (pOutComponentArrayLength == nullptr)
            {
                return nullptr;
            }

            ComponentKey componentKey = IComponentData<T>::GetKey();
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
            ComponentKey key = IComponentData<T>::GetKey();

            AddComponentToParadigmInternal(dataSize, dataAlignment, key);
        }
    };

#if ENABLE_TEST_ECS_TEMPLATED_PARADIGMS
    template <typename T, typename... Ts>
    requires std::is_base_of_v<IComponentData<T>, T> && (std::is_base_of_v<IComponentData<Ts>, Ts> && ...)
    class GenerativeParadigm
        : public Movable
        , public Copyable
    {
    private:
        const void* m_ParadigmMemory = nullptr;
        size_t m_ParadigmMemorySize = 0;
        uint32 m_EntityCount = 0;

        consteval size_t k_SizeOfEntity = TotalSizeOfTypes<T, Ts...>::value;

        union {
            ComponentKey m_ParadigmKeyChain[SAVANNA_ECS_MAX_COMPONENT_PARADIGM_KEYS];
            CompoundComponentKey m_CompoundParadigmKeyChain[
                GetRequiredLengthToFillUnion(
                    sizeof(ComponentKey) * SAVANNA_ECS_MAX_COMPONENT_PARADIGM_KEYS,
                    sizeof(CompoundComponentKey)
                )
            ];
        };

        GenerativeParadigm(const void* pParadigmMemory, const size_t& paradigmMemorySize, bool wipeKeyChain)
            : m_ParadigmMemory(pParadigmMemory)
            , m_ParadigmMemorySize(paradigmMemorySize)
        {
            SAVANNA_ASSERT(pParadigmMemory != nullptr, "Paradigm memory cannot be nullptr.");
            SAVANNA_ASSERT(paradigmMemorySize > 0, "Paradigm memory size must be greater than 0.");

            if (wipeKeyChain)
            {
                memset(m_ParadigmKeyChain, 0, sizeof(ComponentKey) * SAVANNA_ECS_MAX_COMPONENT_PARADIGM_KEYS);
            }
        }

    public:
        GenerativeParadigm(const void* pParadigmMemory, const size_t& paradigmMemorySize) : this(pParadigmMemory, paradigmMemorySize, true) {}

        GenerativeParadigm() = default;

        GenerativeParadigm(const GenerativeParadigm& other)
        {
            m_ParadigmMemory = other.m_ParadigmMemory;
            memcpy(m_ParadigmKeyChain, other.m_ParadigmKeyChain, sizeof(m_ParadigmKeyChain) * sizeof(ComponentKey));
        }

        GenerativeParadigm(GenerativeParadigm&& other)
        {
            m_ParadigmMemory = other.m_ParadigmMemory;
            memcpy(m_ParadigmKeyChain, other.m_ParadigmKeyChain, sizeof(m_ParadigmKeyChain) * sizeof(ComponentKey));
            other.m_ParadigmMemory = nullptr;
            other.m_ParadigmMemorySize = 0;
        }

        template <typename U, typename... Us>
        GenerativeParadigm(GenerativeParadigm<U, Us...>&& other)
        {
            m_ParadigmMemory = other.m_ParadigmMemory;
            other.m_ParadigmMemory = nullptr;
            other.m_ParadigmMemorySize = 0;

            // m_ParadigmKeyChain[componentKey.GetRingIndex()] |= componentKey;

            for (int i = 0; i < 4; ++i)
            {
                m_CompoundParadigmKeyChain
            }

            if (other.m_EntityCount * TotalSizeOfTypes<U, Us...>::value > other.m_ParadigmMemorySize)
            {
                // Acquire a larger pool of memory
                // m_ParadigmMemory = Savanna::Memory::Realloc(GetNextPowerOfTwo(other.m_EntityCount * TotalSizeOfTypes<U, Us...>::value));
                m_ParadigmMemory = SAVANNA_REALLOCATE(other.m_EntityCount * TotalSizeOfTypes<U, Us...>::value);//Memory::Realloc(other.m_EntityCount * TotalSizeOfTypes<U, Us...>::value, );
            }
        }

        ~GenerativeParadigm() {}

    private:
        template <typename U, typename... Us>
        static GenerativeParadigm<U, Us> MutateParadigm(const GenerativeParadigm<T, Ts...>& other)
        {
            return GenerativeParadigm<U, Us...>(std::move(other));
        }
    };
#endif // ENABLE_TEST_ECS_TEMPLATED_PARADIGMS
} // namespace Savanna::Entities
