#pragma once

#include <chrono>

#include "SavannaConfiguration.gen.h"

#include "Types/Strings/FixedString.h"

#define SAVANNA_INSERT_SCOPED_PROFILER(functionName)

#define SAVANNA_FORCE_PROFILING 0

#if SAVANNA_FORCE_PROFILING
#   undef SAVANNA_DEEP_ENGINE_PROFILING
#   define SAVANNA_DEEP_ENGINE_PROFILING 1
#endif

#if SAVANNA_DEEP_ENGINE_PROFILING

#undef SAVANNA_INSERT_SCOPED_PROFILER
/**
 * @brief Inserts a scoped profiler into the code and begins profiling.
 */
#define SAVANNA_INSERT_SCOPED_PROFILER( functionName ) \
    savanna::ProfilerMarker __savanna_ProfilerMarker_funcName = savanna::ProfilerMarker( savanna::FixedString256( #functionName ) ); \
    __savanna_ProfilerMarker_funcName.BeginSample();

#endif

namespace savanna
{
    class HashString;

    class ProfilerMarker
    {
    private:
        bool m_Sampling;
        const char* m_ProfilerMarkerName;

        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_EndTime;

    public:
        ProfilerMarker(const char* markerName);
        ~ProfilerMarker();

        void BeginSample();
        double Poll();
        void EndSample();
    };
} // namespace Savanna
