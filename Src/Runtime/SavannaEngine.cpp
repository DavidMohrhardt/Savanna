#include "SavannaEngine.h"

#include "Memory/MemoryManager.h"
#include "Concurrency/JobSystem/JobManager.h"

namespace Savanna
{
    void InitializeManagers()
    {
        MemoryManager::Initialize();

        Concurrency::JobManager::Initialize();
    }

    void ShutdownManagers()
    {
        Concurrency::JobManager::Shutdown();

        MemoryManager::Shutdown();
    }

    void StartManagers()
    {
        MemoryManager::Start();

        Concurrency::JobManager::Start();
    }

    void StopManagers()
    {
        Concurrency::JobManager::Stop();

        MemoryManager::Stop();
    }
} // namespace Savanna

SAVANNA_EXPORT(void) savanna_initialize_managers()
{
    Savanna::InitializeManagers();
}

SAVANNA_EXPORT(void) savanna_start_managers()
{
    Savanna::StartManagers();
}

SAVANNA_EXPORT(void) savanna_stop_managers()
{
    Savanna::StopManagers();
}

SAVANNA_EXPORT(void) savanna_shutdown_managers()
{
    Savanna::ShutdownManagers();
}
