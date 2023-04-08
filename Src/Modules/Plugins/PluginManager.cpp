#include "PluginManager.h"

#include "Loader/PluginLoader.h"

#include <mutex>

namespace Savanna::Plugin
{
    static std::mutex s_PluginManagerMutex;

    PluginManager::PluginManager() {}

    PluginManager::~PluginManager()
    {
        // We don't own the memory for the plugin we just keep the handles
        for (auto& plugin : m_LoadedPlugins)
        {
            if (!PluginLoader::Get().UnloadPlugin(plugin.second))
            {
                // Console::LogError("Failed to unload plugin: %s", plugin.first);
            }
        }

        PluginLoader::Destroy();
    }

    bool PluginManager::LoadPlugin(const char *const pPluginName, OS::LibraryHandle& outHandle)
    {
        // Check if the plugin is already loaded
        outHandle = OS::k_InvalidLibraryHandle;

        std::lock_guard<std::mutex> lock(s_PluginManagerMutex);
        if (m_LoadedPlugins.find(pPluginName) != m_LoadedPlugins.end())
        {
            // Console::LogError("Plugin already loaded: %s", pPluginName);
            outHandle = m_LoadedPlugins[pPluginName];
            return false;
        }
        else if (PluginLoader::Get().LoadPlugin(pPluginName, outHandle))
        {
            // Add the plugin to the loaded plugins
            m_LoadedPlugins[pPluginName] = outHandle;
        }
        else
        {
            // Console::LogError("Failed to load plugin: %s", pPluginName);
            return false;
        }

        return true;
    }

    bool PluginManager::UnloadPlugin(const char* const pPluginName)
    {
        std::lock_guard<std::mutex> lock(s_PluginManagerMutex);
        if (m_LoadedPlugins.find(pPluginName) != m_LoadedPlugins.end())
        {
            if (!PluginLoader::Get().UnloadPlugin(m_LoadedPlugins[pPluginName]))
            {
                // Console::LogError("Failed to unload plugin: %s", pPluginName);
                return false;
            }

            m_LoadedPlugins.erase(pPluginName);
        }
        else
        {
            // Console::LogError("Plugin not loaded: %s", pPluginName);
            return false;
        }
    }

    bool PluginManager::UnloadPlugin(const OS::LibraryHandle& handle)
    {
        std::lock_guard<std::mutex> lock(s_PluginManagerMutex);
        for (auto& plugin : m_LoadedPlugins)
        {
            if (plugin.second == handle)
            {
                if (!PluginLoader::Get().UnloadPlugin(handle))
                {
                    // Console::LogError("Failed to unload plugin: %s", plugin.first);
                    return false;
                }

                m_LoadedPlugins.erase(plugin.first);
                return true;
            }
        }

        // Console::LogError("Plugin not loaded");
        return false;
    }
} // namespace Savanna::Plugin
