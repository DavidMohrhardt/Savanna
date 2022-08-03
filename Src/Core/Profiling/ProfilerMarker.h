#pragma once

#include <chrono>

#include "ProfilerDefinitions.h"
#include "Types/Strings/HashString.h"

#define SAVANNA_INSERT_SCOPED_PROFILER(functionName)

#if ENABLE_DEEP_ENGINE_PROFILING

#undef SAVANNA_INSERT_SCOPED_PROFILER
/**
 * @brief Inserts a scoped profiler into the code and begins profiling.
 */
#define SAVANNA_INSERT_SCOPED_PROFILER( functionName ) \
    Savanna::ProfilerMarker __savanna_ProfilerMarker_ = Savanna::ProfilerMarker( SAVANNA_HASH_STRING( functionName ) ); \
    __savanna_ProfilerMarker_.BeginSample();

#endif

namespace Savanna
{
    class ProfilerMarker
    {
    private:
        HashString m_ProfilerMarkerName;
        bool m_Sampling;

        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_EndTime;

    public:
        ProfilerMarker(HashString markerName);
        ~ProfilerMarker();

        void BeginSample();
        double Poll();
        void EndSample();
    };
} // namespace Savanna
