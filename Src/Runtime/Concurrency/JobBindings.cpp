#include "Scripting/SavannaScripting.h"
#include "JobSystem.h"

using namespace savanna::concurrency;

DECLARE_SCRIPTING_FUNCTION(int) seSubmitScriptingJobContext(lua_CFunction executeFunc)
{
    // seIJobInterface interface { 0 };
    //     .executeFunc = executeFunc;
    // };
    // SAVANNA_NEW(kSavannaAllocatorKindGeneral, )
    // JobSystem::ScheduleJob
    return 0;
}
