#include "pch.h"
#include <string>
#include <memory>

namespace Savanna
{
#if ENABLE_TODOS // Unimplemented
    static class IDynamicDllPlatformLoader
    {
    public:
        static std::shared_ptr<IDynamicDllPlatformLoader> Init()
        {
            loader = Platform::GetLibraryLoaderInterface();
        }

        bool Load(std::string dllName, std::string path)
        {
            return LoadImpl(path);
        }

    private:
        virtual bool LoadImpl(std::string path);

        static std::shared_ptr<IDynamicDllPlatformLoader> loader;
    };
    

    class LazyDllLoader
    {
    public:
        LazyDllLoader(std::string dllName, std::string path)
        {
            
        }

        ~LazyDllLoader()
        {

        }

    private:
        IDynamicDllPlatformLoader* m_DllPlatformLoader;
    };
#endif
}