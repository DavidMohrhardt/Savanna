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

// Standard includes
#include <assert.h>
#include <mutex>

#include <Utilities/Console.h>

namespace savanna::Entities
{

    inline const uint32 ComponentRegistry::GetTotalNumberOfRegisteredComponentsInternal()
    {
        SAVANNA_READ_CRITICAL_SECTION(m_ComponentTypeMapLock,
        {
            return static_cast<uint32>(m_ComponentTypeMap.size());
        });
    }

    const uint8 ComponentRegistry::GetNumberOfComponentKeySets()
    {
        // Load the current value of the component key counter
        if (ComponentRegistry::Get() != nullptr)
        {
            return (ComponentRegistry::Get()->m_NextAvailableComponentKey.load(std::memory_order_acquire)).m_RingIndex;
        }

        return 0;
    }

    const uint32 ComponentRegistry::GetTotalNumberOfRegisteredComponents()
    {
        // Load the current value of the component key counter
        if (ComponentRegistry::Get() != nullptr)
        {
            return ComponentRegistry::Get()->GetTotalNumberOfRegisteredComponentsInternal();
        }

        return 0;
    }
} // namespace  savanna::Entities
