/**
 * @file ConcurrencyCapabilities.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-05-17
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "SavannaEngine.h"
#include "Utilities/SavannaCoding.h"
#include <Types/Singleton/EmbeddedSingleton.h>

namespace savanna::Concurrency
{
    struct Capabilities : public EmbeddedSingleton<Capabilities>
    {
    protected:
        bool m_SupportsFibers;
        bool m_SupportsCoroutines;

        uint8 m_MaxHardwareThreadCount;

    private:
        friend class EmbeddedSingleton<Capabilities>;

        Capabilities();
        ~Capabilities() = default;

    public:

        SAVANNA_NO_DISCARD static bool IsFiberSystemSupported()
        {
            return Get().m_SupportsFibers;
        }

        SAVANNA_NO_DISCARD static bool IsCoroutineSystemSupported()
        {
            return Get().m_SupportsCoroutines;
        }

        SAVANNA_NO_DISCARD static const uint8 GetMaxHardwareThreadCount()
        {
            return Get().m_MaxHardwareThreadCount;
        }

        SAVANNA_NO_DISCARD static const uint8 GetUnreservedThreadCount()
        {
            return Get().m_MaxHardwareThreadCount - 2;
        }
    };

    inline Capabilities::Capabilities()
    {
        m_SupportsFibers = false;
        m_SupportsCoroutines = false;

        m_MaxHardwareThreadCount = std::thread::hardware_concurrency();
    }

} // namespace savanna::Concurrency
