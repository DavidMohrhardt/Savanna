#include <SavannaEngine.h>

#include <Memory/MemoryManager.h>

#include <Concurrency/ThreadManager.h>
#include <Concurrency/JobManager.h>

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

/**
 * @brief The default order of global managers. This is the order in which they will be initialized, started and the reverse order in which they will be stopped and shutdown.
 *
 * For example:
 * // Initialize is called, initialize in order of declaration
 * MemoryManager::Initialize()
 * JobManager::Initialize()
 * // ...
 *
 * // Start is called, start in order of initialization
 * MemoryManager::Start()
 * JobManager::Start()
 * // ...
 *
 * // Stop is called, stop in reverse order of start
 * JobManager::Stop()
 * MemoryManager::Stop()
 * // ...
 *
 * // Shutdown is called, shutdown in reverse order of initialization
 * JobManager::Shutdown()
 * MemoryManager::Shutdown()
 * // ...
 *
 */
static constexpr GlobalManagerFunctionTable k_DefaultManagerOrder[] = {
    // MEMORY MANAGER MUST BE FIRST THIS IS A DEPENDENCY FOR ALL OTHER MANAGERS
    SAVANNA_GLOBAL_MANAGER_FUNCTION_TABLE_ENTRY(MemoryManager),

    // Put other managers here
    SAVANNA_GLOBAL_MANAGER_FUNCTION_TABLE_ENTRY(Concurrency::ThreadManager)
};

// Ensure that the memory manager is the first entry in the table
static_assert(
    k_DefaultManagerOrder[0].Initialize == MemoryManager::Initialize,
    "MemoryManager must be the first entry in the default manager order table");

static constexpr size_t k_ManagerCount = sizeof(k_DefaultManagerOrder) / sizeof(GlobalManagerFunctionTable);

SAVANNA_EXPORT(void) SavannaInitialize()
{
    SAVANNA_INSERT_SCOPED_PROFILER(SavannaInitialize);
    for (size_t i = 0; i < k_ManagerCount; ++i)
    {
        k_DefaultManagerOrder[i].Initialize();
    }
}

SAVANNA_EXPORT(void) SavannaStart()
{
    SAVANNA_INSERT_SCOPED_PROFILER(SavannaStart);
    for (size_t i = 0; i < k_ManagerCount; ++i)
    {
        k_DefaultManagerOrder[i].Start();
    }
}

SAVANNA_EXPORT(void) SavannaStop()
{
    SAVANNA_INSERT_SCOPED_PROFILER(SavannaStop);
    // Stop in reverse order
    for (size_t i = k_ManagerCount; i > 0; --i)
    {
        k_DefaultManagerOrder[i - 1].Stop();
    }
}

SAVANNA_EXPORT(void) SavannaShutdown()
{
    SAVANNA_INSERT_SCOPED_PROFILER(SavannaShutdown);
    // Shutdown in reverse order
    for (size_t i = k_ManagerCount; i > 0; --i)
    {
        k_DefaultManagerOrder[i - 1].Shutdown();
    }
}
