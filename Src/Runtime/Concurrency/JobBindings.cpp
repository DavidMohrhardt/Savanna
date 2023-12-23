#include "Scripting/SavannaScripting.h"
#include "JobSystem.h"

using namespace savanna::Concurrency;

DECLARE_SCRIPTING_FUNCTION(int) seSubmitScriptingJobContext(lua_CFunction executeFunc)
{
    // se_IJobInterface_t interface { 0 };
    //     .executeFunc = executeFunc;
    // };
    // SAVANNA_NEW(kSavannaAllocatorKindGeneral, )
    // JobSystem::ScheduleJob
    return 0;
}
