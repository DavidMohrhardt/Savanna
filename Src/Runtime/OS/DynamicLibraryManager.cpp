#include "DynamicLibraryManager.h"

namespace Savanna::OS
{
    DynamicLibraryManager::~DynamicLibraryManager()
    {
        for (auto& nameLibPair : m_LoadedLibraries)
        {
            LibraryHandle handle = nameLibPair.second;
#if SAVANNA_WINDOWS
            Windows::TryUnloadDynamicLibrary(handle);
#else
            #error UNSUPPORTED PLATFORM
#endif
        }
    }

    bool DynamicLibraryManager::TryLoadLibrary(const FixedString64 &name)
    {
        LibraryHandle handle;
        {
            std::lock_guard<std::mutex> lock(Get()->m_DataMutex);
            if (Get()->m_LoadedLibraries.find(name.c_str()) != Get()->m_LoadedLibraries.find(name))
            {
                return true;
            }
        }


        bool result =
#if SAVANNA_WINDOWS
            Windows::TryLoadDynamicLibrary(name.c_str(), handle);
#else
            #error UNSUPPORTED PLATFORM
#endif

        if (result)
        {
            std::lock_guard<std::mutex> lock(Get()->m_DataMutex);
            Get()->m_LoadedLibraries[name.c_str()] = handle;
        }

        return result;
    }
}
