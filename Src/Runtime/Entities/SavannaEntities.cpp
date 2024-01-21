#include "SavannaEntities.h"


// Public API implementation

SAVANNA_EXPORT(bool) SavannaEntitiesIsValidComponentKey(const seComponentKey& componentId)
{
    return componentId.m_FullComponentKey != SE_INVALID_COMPONENT_KEY.m_FullComponentKey;
}

SAVANNA_EXPORT(bool) SavannaEntitiesCompareKeys(const seComponentKey& entityKey, const seComponentKey& systemLock)
{
    return entityKey.m_FullComponentKey == systemLock.m_FullComponentKey;
}

SAVANNA_EXPORT(void) SavannaEntitiesRegisterComponent(void* pComponent)
{
    // TODO
}
