/**
 * @file Handle.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-01-08
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <Utilities/SavannaCoding.h>
#include <Types/Memory/CacheLine.h>

/**
 * @brief
 *
 */
typedef struct se_handle
{
#if USE_CACHED_PTRS
    void* m_CachedPtr;
#endif // end USE_CACHED_PTRS
    union
    {
        se_intptr m_Handle;
        struct
        {
            se_uint8 m_ArenaId;
            se_uint8 m_Offset;
        };
    };
} se_handle;

#ifdef __cplusplus

namespace Savanna
{
    struct Handle
    {
    private:
        se_handle m_Handle;

    public:
        operator se_handle() const
        {
            return m_Handle;
        }

        Handle() = default;
        ~Handle() = default;

        Handle(const se_handle& handle) : m_Handle(handle) {}
        Handle(const Handle& other) : m_Handle(other.m_Handle) {}
        Handle(Handle&& other) : m_Handle(other.m_Handle) {}

        Handle& operator=(const se_handle& handle)
        {
            m_Handle = handle;
            return *this;
        }

        Handle& operator=(const Handle& other)
        {
            m_Handle = other.m_Handle;
            return *this;
        }

        Handle& operator=(Handle&& other)
        {
            m_Handle = other.m_Handle;
            return *this;
        }

        auto operator<=>(const Handle& other) const { return m_Handle.m_Handle <=> other.m_Handle.m_Handle; }
    };
} // namespace Savanna

#endif // end __cplusplus
