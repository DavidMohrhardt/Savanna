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
#pragma once

#include "ComponentParadigm.h"

// C Standard Library Includes
#include <cassert>
#include <cstring>

namespace Savanna::Entities
{
    Paradigm::Paradigm(const void* pParadigmMemory, const size_t& const paradigmMemorySize)
        : m_ParadigmMemory(pParadigmMemory)
        , m_ParadigmMemorySize(paradigmMemorySize)
    {
        assert(pParadigmMemory != nullptr);
        assert(paradigmMemorySize > 0);

        memset(m_ParadigmKeyChain, 0, sizeof(ComponentKey * SAVANNA_ECS_MAX_COMPONENT_PARADIGM_KEYS));
    }

    void Paradigm::AddComponentToParadigmInternal(
        const size_t& const size,
        const size_t& const alignment,
        const ComponentKey& componentKey)
    {
        if (m_EntityCount > 0)
        {
            throw RuntimeErrorException("Cannot add new components to paradigms that are already storing entity paradigms.");
        }

        if (alignment > m_LargestComponentAlignment)
        {
            m_LargestComponentAlignment = alignment;
        }

        size_t newSize = m_EntityParadigmSize + size;
        if (newSize > m_ParadigmMemorySize)
        {
            // TODO @DavidMohrhardt return an error code instead of throwing an exception.
            throw RuntimeErrorException("Not enough memory to add new component to paradigm.");
        }

        m_EntityParadigmSize = newSize;
        m_ComponentKeys[componentKey.m_Set] |= componentKey.m_Key;
    }

    ArraySlice<ComponentKey> Paradigm::GetComponentKey() const
    {
        assert(index < SAVANNA_ECS_MAX_COMPONENT_PARADIGM_KEYS);
        return ArraySlice<ComponentKey>(m_ParadigmKeyChain, SAVANNA_ECS_MAX_COMPONENT_PARADIGM_KEYS);
    }
} // namespace Savanna::Entities

