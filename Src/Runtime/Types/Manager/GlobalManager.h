#pragma once

#include <Utilities/SavannaCoding.h>

#include "Types/Singleton/Singleton.h"

#define DEFINE_GLOBAL_MANAGER_FRIENDS_FOR(__className) \
    friend class GlobalManager<__className>; \
    friend class Singleton<__className>;

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
    class GlobalManager : public IGlobalManager, public Singleton<T>
    {
    public:
        static bool Initialize()
        {
            if (!IsInitialized())
            {
                s_Initialized = Singleton<T>::Construct()->InitializeInternal();
            }
            return s_Initialized;
        }

        static void Start()
        {
            if (!IsRunning())
            {
                Singleton<T>::Get()->StartInternal();
                s_Running = true;
            }
        }

        static void Stop()
        {
            if (IsRunning())
            {
                Singleton<T>::Get()->StopInternal();
                s_Running = false;
            }
        }

        static void Shutdown()
        {
            if (IsInitialized())
            {
                Singleton<T>::Get()->ShutdownInternal();
                Singleton<T>::Destroy();
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
