/**
 * @file ComponentParadigm.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief Implementation of the ComponentParadigm class.
 * @version 0.1
 * @date 2022-08-28
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "ComponentParadigm.h"

// C Standard Library Includes
#include <cassert>
#include <cstring>

namespace Savanna::Entities
{
    Paradigm::Paradigm()
    {
        memset(m_ParadigmKeyChain, 0, sizeof(m_ParadigmKeyChain) * sizeof(ComponentKey));
    }

    Paradigm::Paradigm(const Paradigm& other)
    {
        m_ParadigmMemory = other.m_ParadigmMemory;
        memcpy(m_ParadigmKeyChain, other.m_ParadigmKeyChain, sizeof(m_ParadigmKeyChain) * sizeof(ComponentKey));
    }

    Paradigm::Paradigm(Paradigm&& other)
    {
        m_ParadigmMemory = other.m_ParadigmMemory;
        memcpy(m_ParadigmKeyChain, other.m_ParadigmKeyChain, sizeof(m_ParadigmKeyChain) * sizeof(ComponentKey));
        other.m_ParadigmMemory = nullptr;
        other.m_ParadigmMemorySize = 0;
    }

    Paradigm::Paradigm(const void* pParadigmMemory, const size_t& paradigmMemorySize)
        : m_ParadigmMemory(pParadigmMemory)
        , m_ParadigmMemorySize(paradigmMemorySize)
    {
        SAVANNA_ASSERT(pParadigmMemory != nullptr);
        SAVANNA_ASSERT(paradigmMemorySize > 0);

        memset(m_ParadigmKeyChain, 0, sizeof(ComponentKey) * SAVANNA_ECS_MAX_COMPONENT_PARADIGM_KEYS);
    }

    Paradigm::~Paradigm()
    {
        // Release memory
    }

    void Paradigm::AddComponentToParadigmInternal(
        const size_t& size,
        const size_t& alignment,
        const ComponentKey& componentKey)
    {
        if (m_EntityCount > 0)
        {
            throw RuntimeErrorException("Cannot add new components to paradigms that are already storing entity paradigms.");
        }

        size_t newSize = m_EntityParadigmSize + size;
        if (newSize > m_ParadigmMemorySize)
        {
            // TODO @DavidMohrhardt return an error code instead of throwing an exception.
            throw RuntimeErrorException("Not enough memory to add new component to paradigm.");
        }

        m_EntityParadigmSize = newSize;
        m_ParadigmKeyChain[componentKey.GetRingIndex()] |= componentKey;
    }

    ArraySlice<ComponentKey> Paradigm::GetKeyChain() const
    {
        return ArraySlice<ComponentKey>(m_ParadigmKeyChain, SAVANNA_ECS_MAX_COMPONENT_PARADIGM_KEYS);
    }
} // namespace Savanna::Entities

