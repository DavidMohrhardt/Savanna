#include "SavannaEntities.h"


// Public API implementation

SAVANNA_EXPORT(bool) SavannaEntitiesIsValidComponentKey(const se_ComponentKey_t& componentId)
{
    return componentId.m_FullComponentKey != SE_INVALID_COMPONENT_KEY.m_FullComponentKey;
}

SAVANNA_EXPORT(bool) SavannaEntitiesCompareKeys(const se_ComponentKey_t& entityKey, const se_ComponentKey_t& systemLock)
{
    return entityKey.m_FullComponentKey == systemLock.m_FullComponentKey;
}

SAVANNA_EXPORT(void) SavannaEntitiesRegisterComponent(void* pComponent)
{
    // TODO
}
