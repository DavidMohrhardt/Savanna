#pragma once

#include "Utilities/PreprocessorDefinitions/PlatformDefinitions.h"

#if SAVANNA_WINDOWS

#include <Windows.h>

namespace Savanna::OS
{
    using LibraryHandle = HMODULE;

    namespace Windows
    {
        template<typename TFuncPtr>
        requires std::is_function_v<TFuncPtr>
        bool TryGetProcAddressForFunc(LibraryHandle& libraryHandle, const char* funcName, TFuncPtr& outFuncPtr)
        {
            outFuncPtr = (T)GetProcAddress(libraryHandle, funcName);
            return outFuncPtr != nullptr;
        }

        bool TryLoadDynamicLibrary(const char* name, LibraryHandle& outLibraryHandle)
        {
            outLibraryHandle = LoadLibrary(name);
            return outLibraryHandle;
        }

        bool TryUnloadDynamicLibrary(LibraryHandle& handle)
        {
            if (handle)
            {
                FreeLibrary(handle);
                return true;
            }

            return false;
        }
    } // namespace Savanna::OS::Windows
} // namespace Savanna::OS
#endif // SAVANNA_WINDOWS
