#include "ProfilerMarker.h"

#include <utility>

#include "Types/Strings/CRC32HashString.h"

#include "Utilities/Console.h"
#include "Utilities/SavannaCoding.h"

namespace Savanna
{
    // Forward declarations
    class SavannaProfilingManager;

    // Referenced from https://en.cppreference.com/w/cpp/thread/hardware_destructive_interference_size
    // auto is utilized to make the template argument less annoying.
    inline auto Now() SAVANNA_NO_EXCEPT { return std::chrono::high_resolution_clock::now(); }

    ProfilerMarker::ProfilerMarker(FixedString256&& markerName)
        : m_ProfilerMarkerName(std::move(markerName))
        , m_Sampling(false)
        , m_StartTime()
        , m_EndTime()
    {}

    ProfilerMarker::~ProfilerMarker()
    {
        EndSample();
        // ProfilerManager::UnregisterProfilerMarker(this);
    }

    void ProfilerMarker::BeginSample()
    {
        if (!m_Sampling)
        {
            m_Sampling = true;
            m_StartTime = { Now() };
        }
    }

    double ProfilerMarker::Poll()
    {
        if (m_Sampling)
        {
            const std::chrono::duration<double, std::milli> elapsed { Now() - m_StartTime };
            return elapsed.count();
        }
        else
        {
            const std::chrono::duration<double, std::milli> elapsed { m_EndTime - m_StartTime };
            return elapsed.count();
        }
    }

    void ProfilerMarker::EndSample()
    {
        if (m_Sampling)
        {
            m_EndTime = Now();
            m_Sampling = false;
            // ProfilerManager::EndProfilerMarker(this);
            SAVANNA_LOG("[Profiler] {} {}ms Elapsed",
                m_ProfilerMarkerName.c_str(),
                Poll());
        }
    }
}
