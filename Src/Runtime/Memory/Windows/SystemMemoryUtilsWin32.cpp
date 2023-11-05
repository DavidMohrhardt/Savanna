#include <Utilities/SavannaCoding.h>

#include "Memory/SystemMemoryUtils.h"

#if SAVANNA_WINDOWS

#define WIN_32_LEAN_AND_MEAN
#include <windows.h>

namespace Savanna
{
    size_t GetPageSize()
    {
        SYSTEM_INFO systemInfo;
        GetSystemInfo(&systemInfo);
        return systemInfo.dwPageSize;
    }

    size_t GetAllocationGranularity()
    {
        SYSTEM_INFO systemInfo;
        GetSystemInfo(&systemInfo);
        return systemInfo.dwAllocationGranularity;
    }

    size_t GetTotalSystemMemory()
    {
        MEMORYSTATUSEX memoryStatus;
        memoryStatus.dwLength = sizeof(memoryStatus);
        GlobalMemoryStatusEx(&memoryStatus);
        return memoryStatus.ullTotalPhys;
    }
} // namespace Savanna::Windows

#endif // SAVANNA_PLATFORM_WINDOWS
