#pragma once

#include <chrono>

#include "ProfilerDefinitions.h"

#include "Types/Strings/FixedString.h"

#define SAVANNA_INSERT_SCOPED_PROFILER(functionName)

#if ENABLE_DEEP_ENGINE_PROFILING

#undef SAVANNA_INSERT_SCOPED_PROFILER
/**
 * @brief Inserts a scoped profiler into the code and begins profiling.
 */
#define SAVANNA_INSERT_SCOPED_PROFILER( functionName ) \
    Savanna::ProfilerMarker __savanna_ProfilerMarker_ = Savanna::ProfilerMarker( Savanna::FixedString256( functionName  ) ); \
    __savanna_ProfilerMarker_.BeginSample();

#endif

namespace Savanna
{
    class HashString;

    class ProfilerMarker
    {
    private:
        bool m_Sampling;
        FixedString256 m_ProfilerMarkerName;

        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_EndTime;

    public:
        ProfilerMarker(FixedString256&& markerName);
        ~ProfilerMarker();

        void BeginSample();
        double Poll();
        void EndSample();
    };
} // namespace Savanna
