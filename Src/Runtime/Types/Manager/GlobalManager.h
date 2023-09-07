#pragma once

#include <Utilities/SavannaCoding.h>

#include "Types/Singleton/EmbeddedSingleton.h"

#define DEFINE_GLOBAL_MANAGER_FRIENDS_FOR(__className) \
    friend class GlobalManager<__className>; \
    friend class EmbeddedSingleton<__className>;

namespace Savanna
{

    template <typename T>
    class GlobalManager : public EmbeddedSingleton<T>
    {
    public:
        static bool Initialize()
        {
            if (!IsInitialized())
            {
                s_IsInitialized = EmbeddedSingleton<T>::Get().InitializeInternal();
            }
            return s_IsInitialized;
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
                s_IsInitialized = false;
            }
        }

        static bool IsInitialized() { return s_IsInitialized; }
        static bool IsRunning() { return s_IsInitialized && s_IsRunning; }

    protected:
        GlobalManager() = default;
        virtual ~GlobalManager() = default;

        virtual bool InitializeInternal() = 0;
        virtual void StartInternal() = 0;
        virtual void StopInternal() = 0;
        virtual void ShutdownInternal() = 0;

    private:
        inline static bool s_IsInitialized = false;
        inline static bool s_IsRunning = false;
    };
} // namespace Savanna
