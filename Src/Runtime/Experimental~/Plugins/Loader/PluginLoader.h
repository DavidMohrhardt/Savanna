#pragma once
// Core Engine Includes
#include "SavannaEngine.h"
#include "Utilities/SavannaCoding.h"

#include <Types/Strings/FixedString.h>
#include <Types/Singleton/EmbeddedSingleton.h>

#include "LibraryHandle.h"

#include <unordered_map>

namespace Savanna::Plugin
{
    class PluginLoader : public EmbeddedSingleton<PluginLoader>
    {
    protected:
        friend class EmbeddedSingleton<PluginLoader>;

    public:
        bool LoadPlugin(const char* const pPluginName, OS::LibraryHandle& outHandle);
        bool UnloadPlugin(const OS::LibraryHandle& handle);
    };
} // namespace Savanna::Plugin
