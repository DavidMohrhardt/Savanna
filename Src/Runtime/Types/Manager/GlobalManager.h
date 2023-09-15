#pragma once

#include <Utilities/SavannaCoding.h>

#include "Types/Singleton/EmbeddedSingleton.h"

#define DEFINE_GLOBAL_MANAGER_FRIENDS_FOR(__className) \
    friend class GlobalManager<__className>; \
    friend class EmbeddedSingleton<__className>;

namespace Savanna
{
    class IGlobalManager
    {
    protected:
        IGlobalManager() = default;
        virtual ~IGlobalManager() = default;

        virtual bool InitializeInternal() = 0;
        virtual void StartInternal() = 0;
        virtual void StopInternal() = 0;
        virtual void ShutdownInternal() = 0;
    };

    template <typename T>
    class GlobalManager : public EmbeddedSingleton<T>, public IGlobalManager
    {
    public:
        static bool Initialize()
        {
            if (!IsInitialized())
            {
                s_Initialized = EmbeddedSingleton<T>::Get().InitializeInternal();
            }
            return s_Initialized;
        }

        static void Start()
        {
            if (!IsRunning())
                EmbeddedSingleton<T>::Get().StartInternal();
        }

        static void Stop()
        {
            if (IsRunning())
                EmbeddedSingleton<T>::Get().StopInternal();
        }

        static void Shutdown()
        {
            if (IsInitialized())
            {
                EmbeddedSingleton<T>::Get().ShutdownInternal();
                s_Initialized = false;
            }
        }

        static bool IsInitialized() { return s_Initialized; }
        static bool IsRunning() { return s_Initialized && s_Running; }

    protected:
        GlobalManager() = default;
        virtual ~GlobalManager() = default;

    private:
        inline static bool s_Initialized = false;
        inline static bool s_Running = false;
    };
} // namespace Savanna
