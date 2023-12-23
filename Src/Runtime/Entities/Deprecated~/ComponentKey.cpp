/**
 * @file ComponentKey.c
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2022-08-28
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "ComponentKey.h"

static_assert((k_MaxKeyValue & k_ComponentKeyDataTeethMask) > 0, "Component key max value is too large");
// static_assert
static_assert((k_ComponentKeyDataRingIndexMask >> SAVANNA_ECS_KEY_TEETH_BIT_COUNT) == k_MaxSetMaskValue, "Component set max value is too large");

bool SavannaIsValidComponentKey(const ComponentKeyData& componentId)
{
    return componentId.m_FullComponentKey != k_InvalidComponentKey.m_FullComponentKey;
}

bool SavannaCompareKeys(const ComponentKeyData& entityKey, const ComponentKeyData& systemLock)
{
    return entityKey.m_FullComponentKey == systemLock.m_FullComponentKey;
}

namespace savanna::Entities
{
    bool ComponentKey::CompareKey(const ComponentKey& componentKey) const
    {
        return SavannaCompareKeys(m_ComponentKey, componentKey);
    }

    ComponentKeyData ComponentKey::GetKey() const
    {
        return m_ComponentKey;
    }

    se_ComponentKey_t ComponentKey::GetKeyValue() const
    {
        return m_ComponentKey.m_KeyTeeth;
    }

    se_ComponentKeyMask_T ComponentKey::GetRingIndex() const
    {
        return static_cast<se_ComponentKeyMask_T>(m_ComponentKey.m_RingIndex);
    }

    void ComponentKey::SetKey(const ComponentKeyData& componentKey)
    {
        m_ComponentKey = componentKey;
    }

    void ComponentKey::SetKey(const se_ComponentKey_t& componentKey)
    {
        m_ComponentKey.m_FullComponentKey = componentKey;
    }

    void ComponentKey::SetRingIndex(const se_ComponentKeyMask_T& componentKeyRingIndex)
    {
        SAVANNA_ASSERT(componentKeyRingIndex <= k_MaxSetMaskValue, "Component key ring index is too large");
        SAVANNA_ASSERT((componentKeyRingIndex << SAVANNA_ECS_KEY_TEETH_BIT_COUNT) > k_MaxKeyValue, "Component key ring index conflicts with key value");
        m_ComponentKey.m_RingIndex = componentKeyRingIndex;
    }

    void ComponentKey::SetKeyValue(const se_ComponentKey_t& componentKeyValue)
    {
        SAVANNA_ASSERT(componentKeyValue <= k_MaxKeyValue, "Component key value is too large");
        m_ComponentKey.m_KeyTeeth = componentKeyValue;
    }

    void ComponentKey::Invalidate()
    {
        m_ComponentKey = k_InvalidComponentKey;
    }

    bool ComponentKey::IsValid() const
    {
        return SavannaIsValidComponentKey(m_ComponentKey);
    }
} // namespace Savanna
