#pragma once

#include "SavannaEngine.h"
#include "Utilities/SavannaCoding.h"

#include "Types/Singleton/Singleton.h"
#include "Types/Strings/FixedString.h"

#include <concepts>
#include <memory>
#include <type_traits>
#include <unordered_map>

#include "Windows/PlatformLibraryLoader.h"

namespace Savanna::OS
{
    class DynamicLibraryManager : Singleton<DynamicLibraryManager>
    {
    private:
        friend class Singleton<DynamicLibraryManager>;

        // std::unordered_map<const FixedString64, LibraryHandle> m_LoadedLibraries {};
        std::unordered_map<const char*, LibraryHandle> m_LoadedLibraries {};
        std::mutex m_DataLock;

    protected:
        DynamicLibraryManager() = default;
        ~DynamicLibraryManager();

    public:
        static bool TryLoadLibrary(const FixedString64& name);
        // TODO currently unsupported. Libraries stay loaded for the duration of the program
        // static bool TryUnloadLibrary(const FixedString64& name);

        template<typename TFuncPtr>
        requires std::is_function_v<TFuncPtr>
        bool TryGetProcAddressForFunc(const FixedString64& libraryName, const FixedString64& funcName, TFuncPtr& outFuncPtr)
        {
            if (m_LoadedLibraries.find(libraryName) != m_LoadedLibraries.end())
            {
                LibraryHandle handle;

                {
                    std::lock_guard<std::mutex> lock(m_DataMutex);
                    handle = m_LoadedLibraries[libraryName];
                }

#if SAVANNA_WINDOWS
                return Windows::TryGetProcAddressForFunc<T>(handle, funcName.c_str(), &outFuncPtr);
#else
                #error UNSUPPORTED PLATFORM.
#endif
            }
            return nullptr;
        }
    };
} // namespace Savanna::OS
