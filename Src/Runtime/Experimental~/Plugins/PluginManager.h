#pragma once

// Core Engine Includes
#include "SavannaEngine.h"
#include "Utilities/SavannaCoding.h"
#include <Types/Strings/FixedString.h>
#include <Types/Singleton/Singleton.h>

#include "Loader/PluginLoader.h"

namespace Savanna::Plugin
{
    class PluginManager : public Singleton<PluginManager>
    {
    private:
        friend class Singleton<PluginManager>;
        std::unordered_map<FixedString64, OS::LibraryHandle> m_LoadedPlugins;

    public:
        bool LoadPlugin(const char* const pPluginName, OS::LibraryHandle& outHandle);
        bool UnloadPlugin(const char* const pPluginName);
        bool UnloadPlugin(const OS::LibraryHandle& handle);

    private:
        PluginManager();
        ~PluginManager();
    };
} // namespace Savanna::Plugin
