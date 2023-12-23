#include "EntitiesManager.h"

#include <Public/ISavannaEngine.h>

namespace savanna::Entities
{
    EntitiesManager::EntitiesManager() {}
    EntitiesManager::~EntitiesManager() {}

    bool EntitiesManager::InitializeInternal() { return true; }

    void EntitiesManager::StartInternal()
    {
        // SavannaRegisterUpdateFunction(EntitiesManager::Update);
    }

    void EntitiesManager::StopInternal()
    {
        // SavannaUnregisterUpdateFunction(EntitiesManager::Update);
    }

    void EntitiesManager::ShutdownInternal() {}
} // namespace savanna::Entities
