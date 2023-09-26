#include "SavannaEngine.h"

#include "Memory/MemoryManager.h"
#include "Concurrency/JobSystem/JobManager.h"

using namespace Savanna;

struct GlobalManagerFunctionTable
{
    bool (*Initialize)();
    void (*Start)();
    void (*Stop)();
    void (*Shutdown)();
};

#define SAVANNA_GLOBAL_MANAGER_FUNCTION_TABLE_ENTRY(__managerName) \
    GlobalManagerFunctionTable { \
        __managerName::Initialize, \
        __managerName::Start, \
        __managerName::Stop, \
        __managerName::Shutdown \
    }

static constexpr GlobalManagerFunctionTable k_DefaultManagerOrder[] = {
    SAVANNA_GLOBAL_MANAGER_FUNCTION_TABLE_ENTRY(MemoryManager),
    SAVANNA_GLOBAL_MANAGER_FUNCTION_TABLE_ENTRY(Concurrency::JobManager)
};

static constexpr size_t k_ManagerCount = sizeof(k_DefaultManagerOrder) / sizeof(GlobalManagerFunctionTable);

SAVANNA_EXPORT(void) SavannaInitialize()
{
    for (size_t i = 0; i < k_ManagerCount; ++i)
    {
        k_DefaultManagerOrder[i].Initialize();
    }
}

SAVANNA_EXPORT(void) SavannaStart()
{
    for (size_t i = 0; i < k_ManagerCount; ++i)
    {
        k_DefaultManagerOrder[i].Start();
    }
}

SAVANNA_EXPORT(void) SavannaStop()
{
    // Stop in reverse order
    for (size_t i = k_ManagerCount; i > 0; --i)
    {
        k_DefaultManagerOrder[i - 1].Stop();
    }
}

SAVANNA_EXPORT(void) SavannaShutdown()
{
    // Shutdown in reverse order
    for (size_t i = k_ManagerCount; i > 0; --i)
    {
        k_DefaultManagerOrder[i - 1].Shutdown();
    }
}
