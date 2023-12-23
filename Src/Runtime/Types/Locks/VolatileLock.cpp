#include "VolatileLock.h"

namespace savanna
{
    void VolatileLock::Acquire() SAVANNA_NOEXCEPT
    {
        SAVANNA_ASSERT(!m_Locked, "Volatile lock is already acquired. Critical section integrity is compromised.");
        m_Locked = true;
    }

    void VolatileLock::Release() SAVANNA_NOEXCEPT
    {
        SAVANNA_ASSERT(m_Locked, "Volatile lock is already released. Critical section integrity is compromised.");
        m_Locked = false;
    }

    SAVANNA_NO_DISCARD bool VolatileLock::IsAcquired() const SAVANNA_NOEXCEPT
    {
        return m_Locked;
    }

    VolatileLock& VolatileLock::AwaitLock() SAVANNA_NOEXCEPT
    {
        while (IsAcquired())
        {
            // Spin
        }
        Acquire();
        return *this;
    }
} // namespace Savanna
