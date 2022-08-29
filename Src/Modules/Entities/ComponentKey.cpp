/**
 * @file ComponentKey.c
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-28
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "ComponentKey.h"

bool SavannaIsValidComponentKey(const SavannaComponentKey& componentId)
{
    return componentId.m_FullComponentKey != k_InvalidComponentKey.m_FullComponentKey;
}

bool SavannaCompareKeys(const SavannaComponentKey& entityKey, const SavannaComponentKey& systemLock)
{
    return entityKey.m_FullComponentKey == systemLock.m_FullComponentKey;
}
